/*Program to implement stack using linked list*/

#include <stdio.h>
#include <stdlib.h>


/* Structire for a node in a linked list*/
struct node
{
	int item;
	struct node *next;
};

//Function to allocate memory for each node.
struct node * getnode()
{
	struct node *new;
	new = (struct node *) malloc(sizeof(struct node));
	new->next = NULL;
	return new;
}

//Function performing push operation of stack.
struct node * push(struct node *root, struct node *new)
{
	new->next = root; //Make the new node as the new root node
	root = new;
	return root; //return the new root node
}

//Function performing the pop operation of stack.
struct node * pop(struct node *root)
{
	struct node *temp = root;
	if(root == NULL) //If there are no nodes in the linked list
	{
		printf("Stack empty\n\n");
		return NULL;
	}
	printf("%d popped\n",root->item);
	root = root->next; //make the root point to the newxt node
	free(temp); //Free the first node
	return root; //return new root
}

//Function to display the content of the stack
void display(struct node *root)
{
	struct node *temp = root;
	//Printing all the elements in a loop till the root becomes NULL
	while(temp != NULL) 
	{
		printf("%d\t",temp->item);
		temp = temp->next;
	}
	printf("\n");
}

int main()
{
	struct node *root = NULL, *new = NULL;
	int ch;
	int element;
	//To perform  all stack operations
	while(1)
	{
		printf("1. Push\n2. Pop\n3. Display\n4. Exit\n");
		printf("Enter choice : ");
		scanf("%d",&ch);
		
		switch(ch)
		{
			case 1 : printf("Enter element : ");
				 scanf("%d",&element); //Accepting new element of stack
				 new = getnode(); //Allocating memory for the new node
				 new->item = element;
				 root = push(root,new); //Push operation is called
				 printf("%d pushed\n",element);
				 break;
			
			case 2: root = pop(root); //Pop operation is called
				break;

			case 3 : display(root); //Display operation is called
				 break;

			case 4 : exit(0);
				
			default : printf("Invalid option\n");
		}
	}
}
