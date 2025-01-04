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
    //here records[i+1] overwrites records[i]
    for (int i=index; i<table->size-1; i++){
        table->records[i] = table->records[i + 1];
    };
    table->size--
}


void parse_sql(Table *table, const char *sql) {
    if (!sql || strlen(sql) == 0) {
        printf("Empty SQL command.\n");
        return;
    }

    char command[256];
    strncpy(command, sql, sizeof(command));
    command[sizeof(command) - 1] = '\0'; // Ensure null-termination

    char *token = strtok(command, " ");
    if (!token) {
        printf("Invalid SQL command.\n");
        return;
    }

    if (strcasecmp(token, "INSERT") == 0) {
        token = strtok(NULL, " "); // Skip "INTO"
        if (!token || strcasecmp(token, "INTO") != 0) {
            printf("Invalid INSERT syntax: missing 'INTO'.\n");
            return;
        }

        token = strtok(NULL, " "); // Get table name
        if (!token) {
            printf("Invalid INSERT syntax: missing table name.\n");
            return;
        }
        char *tableName = token;

        token = strtok(NULL, " "); // Skip "VALUES"
        if (!token || strcasecmp(token, "VALUES") != 0) {
            printf("Invalid INSERT syntax: missing 'VALUES'.\n");
            return;
        }

        token = strtok(NULL, "("); // Start of values
        if (!token) {
            printf("Invalid INSERT syntax: missing values.\n");
            return;
        }

        int id, age;
        char name[50];
        if (sscanf(token, " %d, '%49[^']', %d", &id, name, &age) != 3) {
            printf("Invalid INSERT values format.\n");
            return;
        }

        insert_record(table, id, name, age);
        printf("Record inserted successfully into table '%s'.\n", tableName);

    } else if (strcasecmp(token, "SELECT") == 0) {
        token = strtok(NULL, " "); // Check for "*"
        if (!token || strcmp(token, "*") != 0) {
            printf("Invalid SELECT syntax: only '*' is supported currently.\n");
            return;
        }

        token = strtok(NULL, " "); // Skip "FROM"
        if (!token || strcasecmp(token, "FROM") != 0) {
            printf("Invalid SELECT syntax: missing 'FROM'.\n");
            return;
        }

        token = strtok(NULL, " "); // Get table name
        if (!token) {
            printf("Invalid SELECT syntax: missing table name.\n");
            return;
        }
        char *tableName = token;

        // Perform selection (currently just selects all records)
        select_all(table);
        printf("Selected all records from table '%s'.\n", tableName);

    } else if (strcasecmp(token, "DELETE") == 0) {
        token = strtok(NULL, " "); // Skip "FROM"
        if (!token || strcasecmp(token, "FROM") != 0) {
            printf("Invalid DELETE syntax: missing 'FROM'.\n");
            return;
        }

        token = strtok(NULL, " "); // Get table name
        if (!token) {
            printf("Invalid DELETE syntax: missing table name.\n");
            return;
        }
        char *tableName = token;

        token = strtok(NULL, " "); // Skip "WHERE"
        if (!token || strcasecmp(token, "WHERE") != 0) {
            printf("Invalid DELETE syntax: missing 'WHERE'.\n");
            return;
        }

        token = strtok(NULL, "="); // Get condition
        if (!token || strcasecmp(token, "ID") != 0) {
            printf("Invalid DELETE syntax: only 'ID=' condition is supported.\n");
            return;
        }

        token = strtok(NULL, " "); // Get ID value
        if (!token) {
            printf("Invalid DELETE syntax: missing ID value.\n");
            return;
        }

        int id;
        if (sscanf(token, "%d", &id) != 1) {
            printf("Invalid DELETE ID value format.\n");
            return;
        }

        delete_record(table, id);
        printf("Record with ID=%d deleted successfully from table '%s'.\n", id, tableName);

    } else {
        printf("Unsupported SQL command: %s\n", token);
    }
};