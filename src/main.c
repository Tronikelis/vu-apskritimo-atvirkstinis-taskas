#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point {
    float x;
    float y;
};

typedef struct CirclePoint {
    struct Point point;
    struct Point center;
};

char* concat_str(char* left, char* right) {
    char* result = malloc(strlen(left) + strlen(right) + 1);
    strcpy(result, left);
    strcat(result, right);
    return result;
}

char** split_str_by(char* _string, char* split_by, int* split_count) {
    char* string = malloc(strlen(_string) + 1);
    strcpy(string, _string);

    int final_count = 0;
    char** final;
    char* token;

    while ((token = strsep(&string, split_by)) != NULL) {
        final_count++;

        if (final_count == 1) {
            final = malloc(sizeof(char*));
        } else {
            final = realloc(final, final_count * sizeof(char*));
        }

        final[final_count - 1] = token;
    }

    *split_count = final_count;

    free(string);
    return final;
}

int main(int argc, char** argv) {
    char* filename = argv[1];
    if (filename == NULL) {
        printf("pass the filename as the first param\n");
        return 1;
    }

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("couldn't open this file\n");
        return 1;
    }

    char buffer[100];
    char* text = malloc(sizeof(char));
    text[0] = '\0';

    while (fgets(buffer, 100, file)) {
        char* newest = concat_str(text, buffer);

        text = realloc(text, strlen(newest) + 1);
        strcpy(text, newest);

        free(newest);
    }

    int i = 0;
    while (text[i++] != '\0') {
        printf("%c", text[i - 1]);
    }

    printf("\n---------------\n");

    int split_count_line = 0;
    char** by_line = split_str_by(text, "\n", &split_count_line);

    int circle_points_count = 0;
    struct CirclePoint* circle_points;

    for (int i = 0; i < split_count_line; i++) {
        int split_count_space = 0;
        char** by_space = split_str_by(by_line[i], " ", &split_count_space);

        // the input text HAS to have 4 numbers
        if (split_count_space <= 3) {
            continue;
        }

        struct Point point;
        point.x = atof(by_space[0]);
        point.y = atof(by_space[1]);

        struct Point center;
        center.x = atof(by_space[2]);
        center.y = atof(by_space[3]);

        struct CirclePoint circle_point;
        circle_point.point = point;
        circle_point.center = center;

        circle_points_count++;

        if (circle_points_count == 1) {
            circle_points = malloc(sizeof(struct CirclePoint));
        } else {
            circle_points =
                realloc(circle_points,
                        circle_points_count * sizeof(struct CirclePoint));
        }

        circle_points[circle_points_count - 1] = circle_point;
    }

    for (int i = 0; i < circle_points_count; i++) {
        struct CirclePoint circle_point = circle_points[i];

        struct Point reverse_point;
        reverse_point.x = -(circle_point.point.x - circle_point.center.x) +
                          circle_point.center.x;

        reverse_point.y = -(circle_point.point.y - circle_point.center.y) +
                          circle_point.center.y;

        printf(
            "point: (%9.2f, %9.2f)\tcenter: (%9.2f, %9.2f)\treverse: (%9.2f, "
            "%9.2f)\n",
            circle_point.point.x, circle_point.point.y, circle_point.center.x,
            circle_point.center.y, reverse_point.x, reverse_point.y);
    }

    free(text);
    free(circle_points);
    free(by_line);

    return 0;
}
