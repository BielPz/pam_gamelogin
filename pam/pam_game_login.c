/*
 * Author: rossano at gmail dot com
 * This file is part of the pamskel distribution
 * (https://gitlab.com/rossanx/pamskel.git).
 * Copyright (c) Rossano Pablo Pinto, 2024.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ----
 */

#include <security/pam_modules.h>
#include <time.h>
#include <stdlib.h>

/*
 * Authentication realm
 */

int PlayFlappy(void);
int PlayCampoMinado(void);
int PlayForca(void);
int PlayRPGMagia(void);
int PlaySnake(void);

int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv )
{
    return PAM_SUCCESS;
}

/*
 * Authentication realm
 */

int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv )
{
	srand(time(NULL));

    int seedJogo = rand() % 5;

    int ganhouGame = 0;

    switch(seedJogo)
    {
    case 0:
        ganhouGame = PlayFlappy();
        break;
    case 1:
        ganhouGame = PlayCampoMinado();
        break;
    case 2:
        ganhouGame = PlayForca();
        break;
    case 3:
	ganhouGame = PlayRPGMagia();
	break;   
    case 4:
	ganhouGame = PlaySnake();
	break;   
    }

    if(ganhouGame)
    {
        return PAM_SUCCESS;
    }
    else
    {
        return PAM_AUTH_ERR;
    }
}
