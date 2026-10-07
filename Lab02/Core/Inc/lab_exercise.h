#ifndef LAB_EXERCISE_H_
#define LAB_EXERCISE_H_

/* Select one lab exercise (1..10), then rebuild the HEX file. */
#ifndef LAB_EXERCISE
#define LAB_EXERCISE 10
#endif

#if LAB_EXERCISE < 1 || LAB_EXERCISE > 10
#error "LAB_EXERCISE must be between 1 and 10"
#endif

#endif
