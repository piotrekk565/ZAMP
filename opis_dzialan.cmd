#define ROTATE_SPEED     30
/*
 *  Przykladowy zestaw polecen
 */
  Begin_Parallel_Actions
  Set   Podstawa.Ramie1 2 0 3 0 0 0      // Polozenie i orientacja obiektu A 
  Set   Podstawa.Ramie1.Ramie2 1 1 0 20 10 0   // Polozenie i orientacja obiektu B
  End_Parallel_Actions
  
  Begin_Parallel_Actions
  Rotate Podstawa.Ramie1.Ramie2 OX ROTATE_SPEED 40
  End_Parallel_Actions

  Begin_Parallel_Actions
  Pause  1000                  /* Zawieszenie na 1 sek. */
  End_Parallel_Actions

  Begin_Parallel_Actions
  Move   Podstawa.Ramie1  10 10
  Rotate Podstawa.Ramie1.Ramie2 OZ ROTATE_SPEED 60 /* Gdyby Rotate i Move wykonywane byloby */
  Move   Podstawa.Ramie1.Ramie2 10 20              /* jednoczesnie, to powodowaloby to      */
  End_Parallel_Actions
                                 /* przesuniecie po luku                  */
