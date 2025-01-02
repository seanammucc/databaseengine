typedef struct {
    int id;
    char name[50];
    int age;
} Record;

typedef struct {
    Record *records;
    int capacity;
    int size;
} Table;

//initialize the database
Table *create_table(int capacity){
    Table *table = (Table *)malloc(sizeof(Table));
    table->records = (Record *)malloc(capacity * sizeof(Record));
    table->capacity = capacity;
    table->size = 0;
    return table;
}

//function to insert a new record

void insert_record(Table *table, int id, const char *name, int age){
    if (table->size >= table->capacity){
        printf("Table is full\n");
        return;
    }

    Record *record = &table->records[table->size++];
    record->id = id;
    strncpy(record->name, name, sizeof(record->name)-1);
    record->name[sizeof(record->name) - 1] = '\0';
    record->age = age;
};

Record *search_record(Table *table, int id){
    for (int i = 0; i < table->size; i++){
        if (table->records[i].id == id){
            return &table->records[i];
        }
    }
    return NULL;
}

void display_records(Table *table) {
    printf("ID\tName\t\tAge\n");
    printf("--------------------------\n");
    for (int i = 0; i < table->size; i++) {
        printf("%d\t%s\t\t%d\n", table->records[i].id, table->records[i].name, table->records[i].age);
    }
}


void delete_record(Table *table, int id){
    int index = -1;
    for (int i = 0; i< table->size; i ++){
        if(table->records[i].id == id){
            index = i;
            break;
        }
    }
     if (index == -1) {
        printf("Record not found.\n");
        return;
    }
    for (int i=index; i<table->size-1; i++){
        table->records[i] = table->records[i + 1];
    };
    table->size--
}
