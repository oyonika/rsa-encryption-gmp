#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

const int MAX=7;

mpz_t p, q, n, g, phi, p_1, q_1, one_mpzt, product, accumulator, pi_x_mod_phi, z, y_mpzt, y_inv, zy, a, b, g_a, ga_y, gga_y, accumulator_b;
long int x[MAX], y, one = 1, three = 3; 

void initialise() {
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(phi);
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_init_set_ui(one_mpzt, one);
    mpz_init_set_ui(g, three);
    printf("\nEnter value of p: ");
    gmp_scanf("%Zd", p);
    printf("\nEnter value of q: ");
    gmp_scanf("%Zd", q);
    mpz_mul(n, p, q);
    mpz_sub(p_1, p, one_mpzt);
    mpz_sub(q_1, q, one_mpzt);
    mpz_mul(phi, p_1, q_1);
    gmp_printf("\n The public parameters (g, N): (%Zd, %Zd)", g, n);
    gmp_printf("\n The private parameters (p, q): (%Zd, %Zd)", p, q);
}

void generate_y_in_x() {
    srand(time(0));
    for (int i=0; i<MAX; i++) {
        int temp = rand();
        if (temp % 2 == 0) {
            i--; 
            continue;
        }
        x[i]=temp;
    }
    y = x[(rand() % 7)]; // generate values between 0 and 7
}

void generate_y_not_in_x() {
    srand(time(0));
    for (int i=0; i<MAX; i++) {
        int temp = rand();
        if (temp % 2 == 0) {
            i--; 
            continue;
        }
        x[i]=temp;
    }
    y = rand();
    for (int j=0; j<MAX; j++) {
        if (y == x[j]) {
            y=rand();
            j=0;
        }
    }
}

void accumulate() {
    mpz_init_set(product, one_mpzt);
    for (int i=0; i<MAX; i++) {
        mpz_mul_ui(product, product, x[i]);
    }
    mpz_powm(pi_x_mod_phi, product, one_mpzt, phi);
    mpz_powm(accumulator, g, pi_x_mod_phi, n);
    gmp_printf("\n\n The accumulator: %Zd", accumulator);
}

void check_y_in_x() {
    mpz_init(z);
    mpz_init_set_ui(y_mpzt, y);
    mpz_cdiv_q(y_inv, one_mpzt, y_mpzt);
    mpz_powm(accumulator, g, pi_x_mod_phi, n);
    mpz_powm(z, accumulator, y_inv, n);
    mpz_powm(zy, z, y_mpzt, n);
    gmp_printf("\n\n The value of z: %Zd", z);
    if(mpz_cmp(zy, accumulator)) {
        printf("\n Result: The value of y belongs in x.");
    } else {
        printf("\n Result: The value of y does not belong in x.");
    }
}

void check_y_not_in_x() {
    mpz_init(a);
    mpz_init(b);
    mpz_invert(a, y_mpzt, n);
    mpz_invert(b, product, n);
    mpz_powm(g_a, g, a, n);
    mpz_powm(ga_y, g_a, y_mpzt, n);
    mpz_mul(gga_y, g, ga_y);
    mpz_powm(accumulator_b, accumulator, b, n);
    gmp_printf("\n\n The value of g.(g^a)^y: %Zd", z);
    if(mpz_cmp(gga_y, accumulator_b)) {
        printf("\n Result: The value of y does not belong in x.");
    } else {
        printf("\n Result: The value of y belongs in x.");
    }
}

int main() {
    int choice;
    do {
        printf("\n\nPress 1 for y belonging in x \n");
        printf("Press 2 for y not belonging in x \n");
        printf("Press 0 to exit the menu \n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: {
                initialise();
                generate_y_in_x();
                accumulate();
                check_y_in_x();
                break;
            }
            case 2: {
                initialise();
                generate_y_not_in_x();
                accumulate();
                check_y_not_in_x();
                break;
            }
            case 0: {
                break;
            }
            default: {
                printf("Invalid input, try again! \n");
                break;
            }
        }
    } while (choice);
    return 0;
}

