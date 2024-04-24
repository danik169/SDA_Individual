#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Document
{
    int doc_id;
    char * doc_name;
    int total_nr_of_lines;
    int left_lines_to_print;
    struct Document * next;
}Document;

typedef struct Printer
{
    int printer_id;
    int time_to_print;
    Document * printer_doc;
    struct Printer * next;
}Printer;

typedef struct Printed_docs
{
    int doc_id;
    char * doc_name;
    int total_nr_of_lines;
    int left_lines_to_print;
    struct Printed_docs * next;
}Printed_docs;

void add_new_doc_in_queue(Document ** queue, int id, char *name, int total_lines,int left_lines){
    Document * new_doc = (Document*)malloc(sizeof(Document));
    new_doc->doc_id = id;
    new_doc->doc_name = strdup(name);
    new_doc->total_nr_of_lines = total_lines;
    new_doc->left_lines_to_print = left_lines;
    new_doc->next = NULL;

    if(*queue == NULL)
    {
        *queue = new_doc;
    }
    else
    {
        Document *temp = *queue;
        while(temp->next)
        {
            temp = temp->next;
        }
        temp->next = new_doc;
    }
}

void delete_a_document_from_queue(Document ** head, int id){
    if(*head == NULL)
    {
        printf("Coada de documente este goala\n");
        return;
    }

    Document * curr_node = *head;
    Document * pre_node = NULL;

    while(curr_node != NULL)
    {
        if(curr_node->doc_id == id)
        {
            if(pre_node == NULL)
            {
                *head = curr_node->next;
                free(curr_node);
                return;
            }
            else
            {
                pre_node->next = curr_node->next;
                free(curr_node);
                return;
            }

        }
        pre_node = curr_node;
        curr_node = curr_node->next;
    }

}

Printer *initialize_printer(FILE *printers){
    int printing_time;
    static int printers_nr = 1;
    Printer *printer = (Printer *)malloc(sizeof(Printer));
    printer->printer_id = printers_nr;
    fscanf(printers, "%d\n", &printing_time);
    printer->time_to_print = printing_time;
    printer->printer_doc = NULL;
    printer->next = NULL;
    printers_nr++;
    return printer;
}

void add_printer(Printer **printer_list,FILE * printers){
    Printer * new_printer = initialize_printer(printers);
    if(*printer_list == NULL)
    {
        *printer_list = new_printer;
    }
    else
    {
        Printer *temp = *printer_list;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_printer;
    }
}

void delete_printer(Printer ** printers_list,int printer_id){
    if(*printers_list == NULL)
    {
        printf("Coada de imprimante este goala\n");
        return;
    }

    Printer * curr_node = *printers_list;
    Printer * pre_iter_printer = NULL;

    while(curr_node)
    {
        if(curr_node->printer_id == printer_id)
        {
            if(pre_iter_printer == NULL)
            {
                *printers_list = curr_node->next;
                free(curr_node);
                return;
            }
            else
            {
                pre_iter_printer->next = curr_node->next;
                free(curr_node);
                return;
            }
        }
        pre_iter_printer = curr_node;
        curr_node = curr_node->next;
    }
}

void distribute_documents(Document ** docs_queue,Printer ** printers_list,Printed_docs ** printed_list){
    if (*docs_queue == NULL || *printers_list == NULL)
    {
        printf("Nu există documente sau imprimante disponibile.\n");
        return;
    }
    Printed_docs * iter_printed_list = NULL;


    Document * curr_doc = *docs_queue;
    Printer * curr_printer = *printers_list;
    Document * temp_doc = NULL;
    Document * selected_doc = NULL;


    int available_printers = 0;

    while(curr_printer != NULL)
    {
        if((curr_printer->printer_doc == NULL ) ||  (curr_printer->printer_doc->left_lines_to_print == 0))
        {
            available_printers++;
        }
        curr_printer = curr_printer->next;
    }
    //printf("available printers %d\n\n",available_printers);

    if(available_printers == 0)
    {
        printf("Nu exista imprimante disponibile\n");
        return;
    }
    else
    {
        while(curr_doc != NULL && available_printers != 0)
        {

            int lines_to_print = -100;
            //curr_printer = *printers_list;
            temp_doc = *docs_queue;
            while(temp_doc != NULL)
            {
                //printf("temp_doc->left_lines_to_print = %d\n",temp_doc->left_lines_to_print);
                if(lines_to_print < temp_doc->left_lines_to_print)
                {
                    lines_to_print = temp_doc->left_lines_to_print;
                    selected_doc = temp_doc;
                }
                temp_doc = temp_doc->next;
            }
            //printf("1       selected doc id %d\n",selected_doc->doc_id);
            if(selected_doc->left_lines_to_print > 0)
            {
                //printf("1   selected->doc id = %d\n",selected_doc->doc_id);
                int max_time_to_print = -1000;
                curr_printer = *printers_list;

                Printer * selected_printer = NULL;
                while(curr_printer != NULL)
                {
                    if(max_time_to_print < curr_printer->time_to_print && curr_printer->printer_doc == NULL)
                    {
                        max_time_to_print = curr_printer->time_to_print;
                        selected_printer = curr_printer;
                    }
                    curr_printer = curr_printer->next;
                }

                if(selected_printer != NULL)
                {
                    selected_printer->printer_doc = selected_doc;
                    //printf("2   selected_printer->printer_doc id = %d\n",selected_printer->printer_doc->doc_id);
                    printf("Documentul = %s(%d/%d) a fost alocat la imprimanta %d cu timpul %d\n",
                           selected_printer->printer_doc->doc_name,
                           selected_printer->printer_doc->left_lines_to_print,
                           selected_printer->printer_doc->total_nr_of_lines,
                           selected_printer->printer_id,
                           selected_printer->time_to_print);
                    available_printers--;
                    //Atribuim documentul selectat in lista de documente procesate

                    Printed_docs * new_doc = (Printed_docs*)malloc(sizeof(Printed_docs));
                    new_doc->doc_id = selected_doc->doc_id;
                    // new_doc->doc_name = malloc(sizeof(selected_doc->doc_name + 1));
                    // strcpy(new_doc->doc_name,selected_doc->doc_name);
                    new_doc->doc_name = strdup(selected_doc->doc_name);
                    new_doc->total_nr_of_lines = selected_doc->total_nr_of_lines;
                    new_doc->left_lines_to_print = selected_doc->left_lines_to_print;
                    new_doc->next = NULL;
                    iter_printed_list = *printed_list;

                    if(iter_printed_list == NULL)
                    {
                        *printed_list = new_doc;
                    }
                    else
                    {
                        while(iter_printed_list->next != NULL)
                        {
                            iter_printed_list = iter_printed_list->next;
                        }
                        iter_printed_list->next = new_doc;
                    }

                    //Scoatem documentul selectat din lista de asteptare il punem in cele procesate
                    Document *pre_iterator = NULL;
                    Document * iterator = *docs_queue;
                    while(iterator != NULL)
                    {
                        if(iterator->doc_id == selected_doc->doc_id)
                        {
                            // printf("2       selected doc id %d\n",selected_doc->doc_id);
                            if(pre_iterator == NULL)
                            {
                                *docs_queue = iterator->next;
                                printf("if docs queue = %d\n",&(*docs_queue));
                                printf("if selected_doc = %d\n",&selected_doc);
                                printf("if iterator = %d\n",&iterator);
                                free(iterator);
                                //break;
                            }
                            else
                            {
                                pre_iterator->next = iterator->next;
                                printf("else docs queue = %d\n",&(*docs_queue));
                                printf("else selected_doc = %d\n",&selected_doc);
                                printf("else iterator = %d\n",&iterator);
                                free(iterator);
                                //break;
                            }
                        }
                        pre_iterator = iterator;
                        iterator = iterator->next;
                        // printf("eiterator & = %d\n",&iterator);
                        //printf("OK\n");
                    }
                }
            }

            curr_doc = curr_doc->next;

        }
    }
}

void update_printers_and_docs_status(Printer ** printers_list){
    Printer * curr_printer = *printers_list;
    int not_printed = 0;
    // Document * print_docs = *docs_queue;
    // Printer * print_printers = *printers_list;

    while(curr_printer)
    {
        if(curr_printer->printer_doc != NULL && curr_printer->printer_doc->left_lines_to_print >= 0)//+
        {
            curr_printer->printer_doc->left_lines_to_print -= curr_printer->time_to_print;

            if(curr_printer->printer_doc->left_lines_to_print <= 0)
            {
                //printf("curr_printer->printer_doc->id = %d",curr_printer->printer_doc->doc_id);
                curr_printer->printer_doc->left_lines_to_print = 0;
                printf("Documentul %s(%d/%d) sa tiparit cu imprimanta %d\n",
                       curr_printer->printer_doc->doc_name,
                       curr_printer->printer_doc->left_lines_to_print,
                       curr_printer->printer_doc->total_nr_of_lines,
                       curr_printer->printer_id);
                //Document * temp_doc = curr_printer->printer_doc;
                curr_printer->printer_doc = NULL;
                //free(temp_doc);
            }
            else
            {
                not_printed++;
            }
        }
        curr_printer = curr_printer->next;
    }
    if(not_printed == 0)
        printf("Documentele sunt in curs de printare\n");
    // if(not_printed)
    //     show_current_status_of_printers_and_docs(docs,print_printers);
}

void show_processed_docs(Printed_docs * processed_list){
    printf("Coada de documente procesate:\n");
    Printed_docs *temp_doc = processed_list;

    while (temp_doc != NULL)
    {
        printf("Document %d: %s (%d/%d)\n",
               temp_doc->doc_id,
               temp_doc->doc_name,
               temp_doc->left_lines_to_print,
               temp_doc->total_nr_of_lines);
        temp_doc = temp_doc->next;
    }
}

void show_current_status_of_printers_and_docs(Document * docs_queue,Printer * printers_list,Printed_docs * printed_docs){
    show_processed_docs(printed_docs);

    printf("Coada de documente neprocesate:\n");
    Document *temp_doc = docs_queue;

    while (temp_doc != NULL)
    {
        printf("Document %d: %s (%d/%d)\n",
               temp_doc->doc_id,
               temp_doc->doc_name,
               temp_doc->left_lines_to_print,
               temp_doc->total_nr_of_lines);
        temp_doc = temp_doc->next;
    }

    printf("\nLista de imprimante:\n");
    Printer *temp_printer = printers_list;

    while (temp_printer != NULL)
    {
        printf("Imprimanta %d: timp de imprimare %d\n",
               temp_printer->printer_id,
               temp_printer->time_to_print);

        temp_printer = temp_printer->next;
    }
}

void free_all_memory(Document **head_docs, Printer **head_print){
    Document * curr1 = *head_docs;
    while(curr1 != NULL)
    {
        Document * temp = curr1;
        curr1 = curr1 -> next;
        free(temp);
    }
    *head_docs = NULL;

    Printer * curr2 = *head_print;
    while(curr2 != NULL)
    {
        Printer * temp = curr2;
        curr2 = curr2 -> next;
        free(temp);
    }
    *head_print = NULL;
}

void show_menu(){
    printf("\nAlegeti o optiune:\n");
    printf("1. Adauga documente\n");
    printf("2. Adauga imprimante\n");
    printf("3. Arata starea curenta a cozii de documente\n");
    printf("4. Anuleaza tiparirea unui element\n");
    printf("5. Elibereaza toata memoria\n");
    printf("6. Distribuie documentele din coada catre imprimante\n");
    printf("7. Actualizeaza starea imprimantelor si a documentelor\n");
    printf("8. Sterge o imprimanta dupa id\n");
    printf("9. Arata documentele procesate\n");
    printf("0. Iesire\n");
}

int main()
{
    Document * docs_queue = NULL;
    Printer * printers_list = NULL;
    Printed_docs * printed_docs = NULL;
    FILE * file = fopen("C:\\Users\\Asus\\Desktop\\SEMINAR SDA\\TASK_2\\docs","r");
    FILE * printer = fopen("C:\\Users\\Asus\\Desktop\\SEMINAR SDA\\TASK_2\\printers", "r");
    int total_docs = 1;
    int option;

   do {
       {
           int nr_of_printers;
           int nr_of_docs;
           char doc_name[30];
           int doc_nr_of_lines;
           int doc_nr_of_left_lines;
           int doc_id_to_delete;
           int printer_id_to_delete;
           show_menu();
           scanf("%d", &option);
           getchar();
           switch (option) {

               case 1:
                   printf("Introdu numarul de documente\n");
                   scanf("%d", &nr_of_docs);
                   getchar();


                   for (int i = 1; i <= nr_of_docs; i++) {
                       int id = total_docs;/*(rand() % 200) + 150*/
                       total_docs++;
                       fscanf(file, "%d,%99[^,],%d,%d\n", &id, doc_name, &doc_nr_of_lines, &doc_nr_of_left_lines);
                       add_new_doc_in_queue(&docs_queue, id, doc_name, doc_nr_of_lines, doc_nr_of_left_lines);
                   }
                   break;

               case 2:
                   printf("Intodu numarul de printer de introdus\n");
                   scanf("%d", &nr_of_printers);
                   getchar();
                   for (int i = 0; i < nr_of_printers; i++) {
                       add_printer(&printers_list, printer);
                   }
                   break;

               case 3:
                   show_current_status_of_printers_and_docs(docs_queue, printers_list, printed_docs);
                   break;

               case 4:
                   printf("Introdu numarul documentului de sters\n");
                   scanf("%d", &doc_id_to_delete);
                   delete_a_document_from_queue(&docs_queue, doc_id_to_delete);
                   break;

               case 5:
                   free_all_memory(&docs_queue, &printers_list);
                   break;

               case 6:
                   distribute_documents(&docs_queue, &printers_list, &printed_docs);
                   break;

               case 7:
                   update_printers_and_docs_status(&printers_list);
                   break;

               case 8:
                   printf("Introdu id-ul imprimantei\n");
                   scanf("%d", &printer_id_to_delete);
                   delete_printer(&printers_list, printer_id_to_delete);
                   break;

               case 9:
                   show_processed_docs(printed_docs);
                   break;

               case 0:
                   free_all_memory(&docs_queue, &printers_list);
                   break;
               default:
                   printf("Enter a different choice\n");

           }
       }
   } while(option != 0);

    fclose(file);
    fclose(printer);
    return 0;
}