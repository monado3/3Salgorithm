#include <stdio.h>
#include <malloc.h>

#define MAXLEN 200000000
#define N_ROW 20
#define SEARCHIDX 5

unsigned int read_pi(unsigned char *arr);

void make_matrix(unsigned int ***mat, unsigned int **base__mat, unsigned int n);

void arr_copy(unsigned int *to_arr, const unsigned int *from_arr, unsigned int n);

unsigned int search(unsigned char *pi_arr, unsigned int n_size_pi, unsigned int len_foundstr, unsigned int n_remain,
                    unsigned int *search_idx_arr, unsigned int n_search_idxs, unsigned int **search_idx_mat);

void output_result(unsigned int *ans_idx_arr);

int main() {
    unsigned char *pi_arr = malloc(sizeof(char) * MAXLEN); // this is 1Bite integer array
    unsigned int n_size_pi, goal_idx;
    unsigned int ans_idx_arr[SEARCHIDX - 1];
    unsigned int **search_idx_mat;
    unsigned int *base_search_idx_mat;
    unsigned int *search_idx_arr;


    n_size_pi = read_pi(pi_arr);
    search_idx_arr = malloc(sizeof(unsigned int) * (n_size_pi / 5));
    make_matrix(&search_idx_mat, &base_search_idx_mat, n_size_pi);
    for (int i = 1; i < SEARCHIDX; ++i) {
        goal_idx = (n_size_pi / SEARCHIDX) * i;
        ans_idx_arr[i - 1] = search(pi_arr, n_size_pi, 0, goal_idx, search_idx_arr, 0, search_idx_mat);
    }
    output_result(ans_idx_arr);


    free(base_search_idx_mat);
    free(search_idx_mat);
    free(pi_arr);
    return 0;
}

unsigned int read_pi(unsigned char *arr) {
    char readline[N_ROW + 2];
    unsigned int num = 0;

    while (fgets(readline, N_ROW + 2, stdin) != NULL) {
        for (unsigned int i = 0; i < N_ROW; ++i) {
            arr[num + i] = (unsigned char) (readline[i] - '0');
        }
        num += N_ROW;
    }
    return num;
}

void make_matrix(unsigned int ***mat, unsigned int **base__mat, unsigned int n) {
    n /= 5;
    *mat = (unsigned int **) malloc(sizeof(unsigned int *) * 10);
    *base__mat = (unsigned int *) malloc(sizeof(unsigned int) * 10 * n);
    for (int i = 0; i < 10; ++i) {
        (*mat)[i] = *base__mat + i * n;
    }
}

void arr_copy(unsigned int *to_arr, const unsigned int *from_arr, unsigned int n) {
    for (unsigned int idx = 0; idx < n; ++idx) {
        to_arr[idx] = from_arr[idx];
    }
}


unsigned int search(unsigned char *pi_arr, unsigned int n_size_pi, unsigned int len_foundstr, unsigned int n_remain,
                    unsigned int *search_idx_arr, unsigned int n_search_idxs, unsigned int **search_idx_mat) {
    unsigned int i, idx;
    unsigned int _cnt_arr[11] = {0};
    unsigned int *cnt_arr = &(_cnt_arr[1]);
    unsigned char number;
    if (len_foundstr == 0) {
        cnt_arr[-1] = 0;
        for (i = 0; i < n_size_pi; ++i) {
            number = pi_arr[i];
            search_idx_mat[number][cnt_arr[number]] = i;
            cnt_arr[number]++;
        }
    } else {
        for (i = 0; i < n_search_idxs; ++i) {
            if ((idx = search_idx_arr[i] + len_foundstr) == n_size_pi) {
                cnt_arr[-1]++;
            } else {
                number = pi_arr[idx];
                search_idx_mat[number][cnt_arr[number]] = search_idx_arr[i];
                cnt_arr[number]++;
            }
        }
    }

    unsigned int accumu_cnt = 0;
    for (char j = -1; j < 10; ++j) {
        accumu_cnt += cnt_arr[j];
        if (accumu_cnt >= n_remain) {
            if (j == -1) {
                return n_size_pi - len_foundstr;
            } else if (cnt_arr[j] == 1) {
                return search_idx_mat[j][0];
            } else {
                len_foundstr++;
                arr_copy(search_idx_arr, search_idx_mat[j], cnt_arr[j]);
                return search(pi_arr, n_size_pi, len_foundstr, n_remain - accumu_cnt + cnt_arr[j], search_idx_arr,
                              cnt_arr[j], search_idx_mat);
            }
        }
    }
}

void output_result(unsigned int *ans_idx_arr) {
    for (int i = 0; i < SEARCHIDX - 1; ++i) {
        printf("the %d index: %9d\n", i + 1, ans_idx_arr[i] + 1);
    }
}
