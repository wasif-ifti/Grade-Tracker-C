#include <stdio.h>
#include <stdlib.h>

struct GradeEntry {
    char studentId[11];
    char assignmentName[21];
    unsigned short grade;
};

struct linked_list_nodes {
    struct GradeEntry data;
    struct linked_list_nodes *current_node;
};

struct linked_list_nodes *head_of_likned_lst = NULL;

int my_strlen(char *s) {
    int i = 0;
    while (*(s + i) != '\0') i++;
    return i;
}

void my_strcpy(char *dest, char *src) {
    int i = 0;
    while (*(src + i) != '\0') {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

int my_strcmp(char *a, char *b) {
    int i = 0;
    while (*(a + i) != '\0' && *(b + i) != '\0') {
        if (*(a + i) != *(b + i)) return 1;
        i++;
    }
    return *(a + i) != *(b + i);
}

void parse_line(char *line, char *sid, char *aname, unsigned short *grade) {
    int i = 0;
    int j = 0;

    while (line[i] != ':' && line[i] != '\0') {
        sid[j] = line[i];
        i++;
        j++;
    }
    sid[j] = '\0';
    i++;

    j = 0;
    while (line[i] != ':' && line[i] != '\0') {
        aname[j] = line[i];
        i++;
        j++;
    }
    aname[j] = '\0';
    i++;

    int g = 0;
    while (line[i] != '\0') {
        g = g * 10 + (line[i] - '0');
        i++;
    }
    *grade = (unsigned short)g;
}

void open_gradebook(char *filename) {
    FILE *database_filr_pointer = fopen(filename, "r");
    if (database_filr_pointer == NULL) {
        printf("error: cannot open file\n");
        exit(1);
    }

    char *file_line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&file_line, &len, database_filr_pointer)) != -1) {
        if (file_line[nread - 1] == '\n') file_line[nread - 1] = '\0';

        char sid[11];
        char aname[21];
        unsigned short g;

        parse_line(file_line, sid, aname, &g);

        struct linked_list_nodes *new_note = malloc(sizeof(struct linked_list_nodes));
        my_strcpy(new_note->data.studentId, sid);
        my_strcpy(new_note->data.assignmentName, aname);
        new_note->data.grade = g;
        new_note->current_node = NULL;

        if (head_of_likned_lst == NULL) {
            head_of_likned_lst = new_note;
        } else {
            struct linked_list_nodes *last = head_of_likned_lst;
            while (last->current_node != NULL) last = last->current_node;
            last->current_node = new_note;
        }
    }

    free(file_line);
    fclose(database_filr_pointer);
}

void save_the_grades(char *filename) {
    FILE *database_filr_pointer = fopen(filename, "w");
    if (database_filr_pointer == NULL) {
        printf("error: cannot save file\n");
        exit(1);
    }

    struct linked_list_nodes *current_node = head_of_likned_lst;
    while (current_node != NULL) {
        fprintf(database_filr_pointer, "%s:%s:%hu\n",
            current_node->data.studentId,
            current_node->data.assignmentName,
            current_node->data.grade);
        current_node = current_node->current_node;
    }

    fclose(database_filr_pointer);
}

void show_everything_bro() {
    printf("Student ID | Assignment Name      | Grade\n");
    printf("-----------------------------------------\n");
    struct linked_list_nodes *current_node = head_of_likned_lst;
    while (current_node != NULL) {
        printf("%s | %-20s | %hu\n",
            current_node->data.studentId,
            current_node->data.assignmentName,
            current_node->data.grade);
        current_node = current_node->current_node;
    }
}

void crunch_numbers(char *aname) {
    struct linked_list_nodes *current_node = head_of_likned_lst;
    int found = 0;
    int min = 101;
    int max = -1;
    int total = 0;
    int count = 0;

    while (current_node != NULL) {
        if (my_strcmp(current_node->data.assignmentName, aname) == 0) {
            found = 1;
            int g = current_node->data.grade;
            if (g < min) min = g;
            if (g > max) max = g;
            total += g;
            count++;
        }
        current_node = current_node->current_node;
    }

    if (found == 0) {
        printf("error: assignment not found\n");
        return;
    }

    double mean = (double)total / count;
    printf("Grade statistics for %s\n", aname);
    printf("Min:  %d\n", min);
    printf("Max:  %d\n", max);
    printf("Mean: %.2f\n", mean);
}

void add_new_grade(char *sid, char *aname, unsigned short g, char *filename) {
    struct linked_list_nodes *current_node = head_of_likned_lst;
    while (current_node != NULL) {
        if (my_strcmp(current_node->data.studentId, sid) == 0 &&
            my_strcmp(current_node->data.assignmentName, aname) == 0) {
            printf("error: entry already exists\n");
            return;
        }
        current_node = current_node->current_node;
    }

    struct linked_list_nodes *new_note = malloc(sizeof(struct linked_list_nodes));
    my_strcpy(new_note->data.studentId, sid);
    my_strcpy(new_note->data.assignmentName, aname);
    new_note->data.grade = g;
    new_note->current_node = NULL;

    if (head_of_likned_lst == NULL) {
        head_of_likned_lst = new_note;
    } else {
        struct linked_list_nodes *last = head_of_likned_lst;
        while (last->current_node != NULL) last = last->current_node;
        last->current_node = new_note;
    }

    save_the_grades(filename);
}

void kick_out_grade(char *sid, char *aname, char *filename) {
    if (head_of_likned_lst == NULL) {
        printf("error: entry not found\n");
        return;
    }

    if (my_strcmp(head_of_likned_lst->data.studentId, sid) == 0 &&
        my_strcmp(head_of_likned_lst->data.assignmentName, aname) == 0) {
        struct linked_list_nodes *to_delete = head_of_likned_lst;
        head_of_likned_lst = head_of_likned_lst->current_node;
        free(to_delete);
        save_the_grades(filename);
        return;
    }

    struct linked_list_nodes *prev = head_of_likned_lst;
    struct linked_list_nodes *curr = head_of_likned_lst->current_node;

    while (curr != NULL) {
        if (my_strcmp(curr->data.studentId, sid) == 0 &&
            my_strcmp(curr->data.assignmentName, aname) == 0) {
            prev->current_node = curr->current_node;
            free(curr);
            save_the_grades(filename);
            return;
        }
        prev = curr;
        curr = curr->current_node;
    }

    printf("error: entry not found\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("error: please provide a database file\n");
        return 1;
    }

    open_gradebook(argv[1]);

    char *what_user_want = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&what_user_want, &len, stdin)) != -1) {
        if (what_user_want[nread - 1] == '\n') {
            what_user_want[nread - 1] = '\0';
            nread--;
        }

        int where_space_is = 0;
        while (what_user_want[where_space_is] != ' ' &&
               what_user_want[where_space_is] != '\0') {
            where_space_is++;
        }

        char *the_rest = what_user_want + where_space_is + 1;
        what_user_want[where_space_is] = '\0';
        char *the_command = what_user_want;

        if (my_strcmp(the_command, "print") == 0) {
            show_everything_bro();
        } else if (my_strcmp(the_command, "stats") == 0) {
            crunch_numbers(the_rest);
        } else if (my_strcmp(the_command, "add") == 0) {
            char sid[11];
            char aname[21];
            unsigned short g;
            parse_line(the_rest, sid, aname, &g);
            add_new_grade(sid, aname, g, argv[1]);
        } else if (my_strcmp(the_command, "remove") == 0) {
            char sid[11];
            char aname[21];
            int i = 0;
            int j = 0;
            while (the_rest[i] != ':' && the_rest[i] != '\0') {
                sid[j] = the_rest[i];
                i++;
                j++;
            }
            sid[j] = '\0';
            i++;
            j = 0;
            while (the_rest[i] != '\0') {
                aname[j] = the_rest[i];
                i++;
                j++;
            }
            aname[j] = '\0';
            kick_out_grade(sid, aname, argv[1]);
        } else {
            printf("Invalid argument\n");
        }
    }

    free(what_user_want);

    save_the_grades(argv[1]);

    struct linked_list_nodes *current_node = head_of_likned_lst;
    while (current_node != NULL) {
        struct linked_list_nodes *next = current_node->current_node;
        free(current_node);
        current_node = next;
    }

    return 0;
}