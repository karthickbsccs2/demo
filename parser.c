/* Program to parse different tables and display the contentof the tables*/

#include<stdio.h>
#include<stdlib.h>
#define HEADER_LENGTH 8
#define HEADER_LENGTH_AFTER_SECTION_LENGTH 5
#define CRC_LENGTH 4
#define PAT_RECORD_LENGTH 4


FILE *fp; /* File pointer to read the content of the file having the table*/

struct header /* Header of every table occupying 8 bytes*/
{
	unsigned table_id:8;
	unsigned section_syntax_indicator:1;
	unsigned reserved_future_use:1;
	unsigned reserved1:2;
	unsigned section_length:12;
	unsigned sub_table_id:16;
	unsigned reserved2:2;
	unsigned version_number:5;
	unsigned current_next_indicator:1;
	unsigned section_number:8;
	unsigned last_section_number:8;
};

struct PAT /*Structure for storing PAT fields*/
{
	unsigned program_number:16;
	unsigned reserved:3;
	unsigned program_map_pid:13;
	unsigned CRC:32;
};

struct PMT/*Structure for storing PMT fields*/
{
	unsigned reserved1:3;
	unsigned PCR_pid:13;
	unsigned reserved2:4;
	unsigned program_info_length:12;
};

struct PMT_info /*Structure for storing information about each program in PMT */
{
	unsigned stream_type:8;
	unsigned resreved3:3;
	unsigned elementary_pid:13;
	unsigned reserved4:4;
	unsigned ES_info_length:12;
	unsigned CRC:32;
};

struct NIT /*Structure for storing NIT fields */
{
	unsigned reserved:4;
	unsigned networks_descriptors_length:12;
	unsigned reserved_future_use:4;
	unsigned transport_stream_loop_length:12;
};

struct NIT_info /* Structure for storing information about each transport stream in NIT */
{
	unsigned transport_stream_id:16;
	unsigned original_network_id:16;
	unsigned reserved_future_use:4;
	unsigned transport_descriptors_length:12;
};

/*Read the content of the file into a character array */
void readFile(unsigned char *table)
{
	int i = 0;
	char c; 
	
	while(!feof(fp))
	{
		fscanf(fp,"%02x", &table[i]);
		//printf("%02x\t",table[i]);
		i++;
		c = fgetc(fp);	
	}
}

/* Function to parse the 8 bytes header of every table */
void parseHeader(unsigned char *table, struct header *head)
{
	head->table_id = table[0];
	//printf("%d\n",head->table_id);
	head->section_syntax_indicator = ((table[1] & (0x80)) != 0);
	//printf("%d\n",head->section_syntax_indicator);
	head->section_length = ((table[1] & (0x0F)) << 8) | (table[2]);
	//printf("%d\n",head->section_length);
	head->sub_table_id = (table[3] << 8) | (table[4]);
	head->version_number = (table[5] & (0x3E)) >> 1;
	head->current_next_indicator = (table[5] & (0x01) != 0);
	head->section_number = table[6];
	head->last_section_number = table[7];

}

/*Function to display the header details */
void displayHeader(struct header *head)
{
	printf("\n\n\t\tHEADER\n\n");
	printf("Table id : %x\n",head->table_id);
	printf("Section syntax indicator : %d\n",head->section_syntax_indicator);
	printf("Section length : %d\n", head->section_length);
	printf("Sub table id : %x\n", head->sub_table_id);
	printf("Version number : %d\n", head->version_number);
	printf("Current next indicator : %d\n", head->current_next_indicator);
	printf("Section number : %d\n", head->section_number);
	printf("Last section number : %d\n", head->last_section_number);	
}

/* function to display PAT info */
void displayPAT(struct PAT pat_record)
{
	printf("%d\t\t%x\n",pat_record.program_number, pat_record.program_map_pid);
}

/* Function to parse PAT */
void parse_PAT(struct header* head, char* table)
{
	int n,i,j;
	struct PAT pat_record;
	n = (head->section_length - CRC_LENGTH - HEADER_LENGTH_AFTER_SECTION_LENGTH) / PAT_RECORD_LENGTH;
	j = HEADER_LENGTH;
	printf("\nPROGRAM NUMBER\tPID\n");
	for(i = 0; i<n; i++)
	{
		pat_record.program_number = (table[j] << 8) | table[j+1] ;
		j+=2;
		pat_record.program_map_pid = ((table[j] & (0x1F)) << 3) | table[j+1];
		j+=2;
		displayPAT(pat_record);
	}
}

void descriptor()
{

}

/* Function to display PMT fields */
void display_PMT(struct PMT pmt_record)
{
	printf("PCR pid : %d\n",pmt_record.PCR_pid);
	printf("Program info length : %d\n",pmt_record.program_info_length);
}

/* Function to display information about each program in PMT */
void display_PMT_info(struct PMT_info pmt_info)
{
	printf("\nStream type : %d\n",pmt_info.stream_type);
	printf("Elementary PID : %d\n",pmt_info.elementary_pid);
	printf("ES info length : %d\n",pmt_info.ES_info_length);
}

/* Function to parse PMT */
void parse_PMT(struct header* head, char* table)
{
	int n,n1,n2,i,j;
	struct PMT pmt_record;
	struct PMT_info pmt_info;
	j = HEADER_LENGTH;
	pmt_record.PCR_pid = ((table[j] & (0x1F)) << 3) | table[j+1];
	j+=2;
	pmt_record.program_info_length = ((table[j] & (0x0F)) << 4) | table[j+1];
	j+=2;
	display_PMT(pmt_record);
	n = pmt_record.program_info_length;
	for(i=0; i<n; i++)
	{
		descriptor();
		j++;
	}
	
	n1 = head->section_length - CRC_LENGTH;
	while(j < n1)
	{
		pmt_info.stream_type = table[j];
		j++;
		pmt_info.elementary_pid = ((table[j] & (0x1F)) << 3) | table[j+1];
		j+=2;
		pmt_info.ES_info_length = ((table[j] & (0x0F)) << 4) | table[j+1];
		j+=2;
		n2 = pmt_info.ES_info_length;
		display_PMT_info(pmt_info);
		for(i=0; i<n2; i++)
		{
			descriptor();
			j++;
		}
	}
}

/* Function to display fields of NIT */
void display_NIT(struct NIT nit_record)
{
	printf("Networks descriptors length : %d\n",nit_record.networks_descriptors_length);
	printf("Transport stream loop length : %d\n", nit_record.transport_stream_loop_length);
}

/* Function to display the information about each transport stream in NIT */
void display_NIT_info(struct NIT_info nit_info)
{
	printf("\nTransport stream id : %d\n",nit_info.transport_stream_id);
	printf("Original network id : %d\n", nit_info.original_network_id);
	printf("Transport descriptors length : %d\n", nit_info.transport_descriptors_length);
}

/* Function to parse NIT */
void parse_NIT(struct header* head, char* table)
{
	int i,j,n,n1,n2;
	struct NIT nit_record;
	struct NIT_info nit_info;
	j = HEADER_LENGTH;
	nit_record.networks_descriptors_length = ((table[j] & (0x0F)) << 4) | table[j+1];
	j+=2;
	n = nit_record.networks_descriptors_length;
	for(i=0; i<n; i++)
        {
                descriptor();
                j++;
        }
	nit_record.transport_stream_loop_length = ((table[j] & (0x0F)) << 4) | table[j+1];
	j+=2;
	display_NIT(nit_record);
	n1 = head->section_length - CRC_LENGTH;
	while(j < n1)
	{
		nit_info.transport_stream_id = (table[j] << 8) | table[j+1];
		j+=2;
		nit_info.original_network_id = (table[j] << 8) | table[j+1];
		j+=2;
		n2 = nit_info.transport_descriptors_length = ((table[j] & (0x0F)) << 4) | table[j+1];
		j+=2;
		display_NIT_info(nit_info);
		for(i=0; i<n2; i++)
                {
                        descriptor();
                        j++;
                }
	}

}

int main()
{
	char file[15];
	unsigned char table[2000];
	struct header head;
	printf("Enter the file name to parse : ");
	scanf("%s",file);
	fp = fopen(file,"r");
	readFile(table);
	parseHeader(table,&head);
	displayHeader(&head);
	switch(head.table_id) /*check for table id to proceed with parsing */
	{
		case 0x00 : printf("\n\t\tPAT\n\n");
			    parse_PAT(&head, table);
			    break;

		case 0x02 : printf("\n\t\tPMT\n\n");
			    parse_PMT(&head, table);
			    break;

		case 0x40 : printf("\n\t\tNIT\n\n");
			    parse_NIT(&head, table);
			    break;
	}
}
