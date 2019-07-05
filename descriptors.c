/* TS header details */
transport_packet()
{
	sync_byte 						8 bslbf
	transport_error_indicator 			1 bslbf
	payload_unit_start_indicator 			1 bslbf
	transport_priority 					1 bslbf
	PID 								13 uimsbf
	transport_scrambling_control 		2 bslbf
	adaptation_field_control 			2 bslbf
	continuity_counter 					4 uimsbf
	if(adaptation_field_control = = '10' || adaptation_field_control = = '11')
	{
		adaptation_field()
	}
	if(adaptation_field_control = = '01' || adaptation_field_control = = '11')
	{
		for (i = 0; i < N; i++)
		{
			data_byte 8 bslbf
		}
	}
}

/* PAT details */
program_association_section()
{
	table_id 						8 uimsbf
	section_syntax_indicator 		1 bslbf
	'0' 							1 bslbf
	reserved 						2 bslbf
	section_length					12 uimsbf
	transport_stream_id 			16 uimsbf
	reserved 						2 bslbf
	version_number 				5 uimsbf
	current_next_indicator 			1 bslbf
	section_number 				8 uimsbf
	last_section_number 			8 uimsbf
	for (i = 0; i < N; i++)
	{
		program_number 			16 uimsbf
		reserved 					3 bslbf
		if (program_number = = '0')
		{
			network_PID 			13 uimsbf
		}
		else
		{
			program_map_PID 	13 uimsbf
		}
	}
	CRC_32 32 rpchof
}

event_information_section()
{
	table_id 8 uimsbf
	section_syntax_indicator 1 bslbf
	reserved_future_use 1 bslbf
	reserved 2 bslbf
	section_length 12 uimsbf
	service_id 16 uimsbf
	reserved 2 bslbf
	version_number 5 uimsbf
	current_next_indicator 1 bslbf
	section_number 8 uimsbf
	last_section_number 8 uimsbf
	transport_stream_id 16 uimsbf
	original_network_id 16 uimsbf
	segment_last_section_number 8 uimsbf
	last_table_id 8 uimsbf
	for(i=0;i<N;i++){
	event_id 16 uimsbf
	start_time 40 bslbf
	duration 24 uimsbf
	running_status 3 uimsbf
	free_CA_mode 1 bslbf
	descriptors_loop_length 12 uimsbf
	for(i=0;i<N;i++){
	descriptor()
	}
	}
	CRC_32 32 rpchof
}


TS_program_map_section()
{
	table_id 						8 uimsbf
	section_syntax_indicator 		1 bslbf
	'0' 							1 bslbf
	reserved 						2 bslbf
	section_length 				12 uimsbf
	program_number 				16 uimsbf
	reserved 						2 bslbf
	version_number 				5 uimsbf
	current_next_indicator 			1 bslbf
	section_number 				8 uimsbf
	last_section_number 			8 uimsbf
	reserved 						3 bslbf
	PCR_PID 					13 uimsbf
	reserved 						4 bslbf
	program_info_length 			12 uimsbf

	for (i = 0; i < N; i++)
	{
		descriptor()
	}
	for (i = 0; i < N1; i++) {
	stream_type 8 uimsbf
	reserved 3 bslbf
	elementary_PID 13 uimsbf
	reserved 4 bslbf
	ES_info_length 12 uimsbf
	for (i = 0; i < N2; i++) {
	descriptor()
	}
	}
	CRC_32 32 rpchof
}

/* Section details */
ISO_IEC_14496_section()  /* page no. 108 of 13818-1*/
{
	table_id 						8 uimsbf	|table_id 					8 uimsbf	|table_id 					8 uimsbf
	section_syntax_indicator 		1 bslbf		|section_syntax_indicator 	1 bslbf		|section_syntax_indicator 	1 bslbf		/* 0 -> Private section, 1 -> other stream type */
	private_indicator 				1 bslbf		|'0' 						1 bslbf		|'0' 						1 bslbf
	reserved 						2 bslbf		|reserved 					2 bslbf		|reserved 					2 bslbf
	___________________________________________	|_______________________________________|_____________________________________
	ISO_IEC_14496_section_length 	12 uimsbf	|section_length				12 uimsbf	|section_length 			12 uimsbf
	___________________________________________	|_______________________________________|_____________________________________
	table_id_extension 				16 uimsbf	|transport_stream_id 		16 uimsbf	|program_number 			16 uimsbf /* Most important */
	___________________________________________	|_______________________________________|_____________________________________
	reserved 						2 bslbf		|reserved 					2 bslbf		|reserved 					2 bslbf
	version_number 					5 uimsbf	|version_number 			5 uimsbf	|version_number 			5 uimsbf
	current_next_indicator 			1 bslbf		|current_next_indicator 	1 bslbf		|current_next_indicator 	1 bslbf
	section_number 					8 uimsbf	|section_number 			8 uimsbf	|section_number 			8 uimsbf
	last_section_number 			8 uimsbf	|last_section_number 		8 uimsbf	|last_section_number 		8 uimsbf
												|										|reserved 					3 bslbf
												|										|PCR_PID 					13 uimsbf
												|										|reserved 					4 bslbf
												|										|program_info_length 		12 uimsbf



	if (PMT_has_SL_descriptor(current_PID))
	{
		SL_Packet()
	}
	else if (PMT_has_FMC_descriptor(current_PID))
	{
		for (i=1; i<N; i++)
			FlexMuxPacket()
	}
	else
	{
		for (i=1; i<N; i++)
			reserved 				8 bslbf
	}
	CRC_32 32 rpchof
}
____________________________________________________
SLNO		DESCRIPTION		TABLE ID			PID
____________________________________________________
1			PAT				0x00			0x00
2			PMT				0x02			undefined
3			NIT				0x40 0x41		0x10
4			CAT				0x01			-
____________________________________________________

