//
// Created by Anthony Lesch on 7/31/24.
//

#pragma once

/**
 * Return whether x is prime or not
 *
 * Returns:
 *   1  - prime
 *   0  - not prime
 *   -1 - undefined (i.e. x < 2)
 */
int IsPrime(int x);

/**
 * Return the next prime after x, or x if x is prime
 */
int NextPrime(int x);
