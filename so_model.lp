\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 500 alpha(0,0,0) + 3000 alpha(0,0,1) + 3000 alpha(0,0,2)
       + 3000 alpha(0,0,3) + 3000 alpha(0,0,4) + 4200 alpha(0,1,0)
       + 700 alpha(0,1,1) + 4200 alpha(0,1,2) + 4200 alpha(0,1,3)
       + 4200 alpha(0,1,4) + 6000 alpha(0,2,0) + 6000 alpha(0,2,1)
       + 1000 alpha(0,2,2) + 6000 alpha(0,2,3) + 6000 alpha(0,2,4)
       + 3000 alpha(0,3,0) + 3000 alpha(0,3,1) + 3000 alpha(0,3,2)
       + 500 alpha(0,3,3) + 3000 alpha(0,3,4) + 1500 alpha(1,0,0)
       + 4000 alpha(1,0,1) + 4000 alpha(1,0,2) + 4000 alpha(1,0,3)
       + 4000 alpha(1,0,4) + 5600 alpha(1,1,0) + 2100 alpha(1,1,1)
       + 5600 alpha(1,1,2) + 5600 alpha(1,1,3) + 5600 alpha(1,1,4)
       + 8000 alpha(1,2,0) + 8000 alpha(1,2,1) + 3000 alpha(1,2,2)
       + 8000 alpha(1,2,3) + 8000 alpha(1,2,4) + 4000 alpha(1,3,0)
       + 4000 alpha(1,3,1) + 4000 alpha(1,3,2) + 1500 alpha(1,3,3)
       + 4000 alpha(1,3,4) + 1000 alpha(2,0,0) + 3500 alpha(2,0,1)
       + 3500 alpha(2,0,2) + 3500 alpha(2,0,3) + 3500 alpha(2,0,4)
       + 4900 alpha(2,1,0) + 1400 alpha(2,1,1) + 4900 alpha(2,1,2)
       + 4900 alpha(2,1,3) + 4900 alpha(2,1,4) + 7000 alpha(2,2,0)
       + 7000 alpha(2,2,1) + 2000 alpha(2,2,2) + 7000 alpha(2,2,3)
       + 7000 alpha(2,2,4) + 3500 alpha(2,3,0) + 3500 alpha(2,3,1)
       + 3500 alpha(2,3,2) + 1000 alpha(2,3,3) + 3500 alpha(2,3,4)
       + 1000 alpha(3,0,0) + 3500 alpha(3,0,1) + 3500 alpha(3,0,2)
       + 3500 alpha(3,0,3) + 3500 alpha(3,0,4) + 4900 alpha(3,1,0)
       + 1400 alpha(3,1,1) + 4900 alpha(3,1,2) + 4900 alpha(3,1,3)
       + 4900 alpha(3,1,4) + 7000 alpha(3,2,0) + 7000 alpha(3,2,1)
       + 2000 alpha(3,2,2) + 7000 alpha(3,2,3) + 7000 alpha(3,2,4)
       + 3500 alpha(3,3,0) + 3500 alpha(3,3,1) + 3500 alpha(3,3,2)
       + 1000 alpha(3,3,3) + 3500 alpha(3,3,4)
Subject To
 con(0):   alpha(0,0,0) + alpha(0,0,1) + alpha(0,0,2) + alpha(0,0,3)
           + alpha(0,0,4) + alpha(1,0,0) + alpha(1,0,1) + alpha(1,0,2)
           + alpha(1,0,3) + alpha(1,0,4) + alpha(2,0,0) + alpha(2,0,1)
           + alpha(2,0,2) + alpha(2,0,3) + alpha(2,0,4) + alpha(3,0,0)
           + alpha(3,0,1) + alpha(3,0,2) + alpha(3,0,3) + alpha(3,0,4)  = 1
 con(1):   alpha(0,1,0) + alpha(0,1,1) + alpha(0,1,2) + alpha(0,1,3)
           + alpha(0,1,4) + alpha(1,1,0) + alpha(1,1,1) + alpha(1,1,2)
           + alpha(1,1,3) + alpha(1,1,4) + alpha(2,1,0) + alpha(2,1,1)
           + alpha(2,1,2) + alpha(2,1,3) + alpha(2,1,4) + alpha(3,1,0)
           + alpha(3,1,1) + alpha(3,1,2) + alpha(3,1,3) + alpha(3,1,4)  = 1
 con(2):   alpha(0,2,0) + alpha(0,2,1) + alpha(0,2,2) + alpha(0,2,3)
           + alpha(0,2,4) + alpha(1,2,0) + alpha(1,2,1) + alpha(1,2,2)
           + alpha(1,2,3) + alpha(1,2,4) + alpha(2,2,0) + alpha(2,2,1)
           + alpha(2,2,2) + alpha(2,2,3) + alpha(2,2,4) + alpha(3,2,0)
           + alpha(3,2,1) + alpha(3,2,2) + alpha(3,2,3) + alpha(3,2,4)  = 1
 con(3):   alpha(0,3,0) + alpha(0,3,1) + alpha(0,3,2) + alpha(0,3,3)
           + alpha(0,3,4) + alpha(1,3,0) + alpha(1,3,1) + alpha(1,3,2)
           + alpha(1,3,3) + alpha(1,3,4) + alpha(2,3,0) + alpha(2,3,1)
           + alpha(2,3,2) + alpha(2,3,3) + alpha(2,3,4) + alpha(3,3,0)
           + alpha(3,3,1) + alpha(3,3,2) + alpha(3,3,3) + alpha(3,3,4)  = 1
 con(4):   alpha(0,4,0) + alpha(0,4,1) + alpha(0,4,2) + alpha(0,4,3)
           + alpha(0,4,4) + alpha(1,4,0) + alpha(1,4,1) + alpha(1,4,2)
           + alpha(1,4,3) + alpha(1,4,4) + alpha(2,4,0) + alpha(2,4,1)
           + alpha(2,4,2) + alpha(2,4,3) + alpha(2,4,4) + alpha(3,4,0)
           + alpha(3,4,1) + alpha(3,4,2) + alpha(3,4,3) + alpha(3,4,4)  = 1
 cap(0,0): 50 alpha(0,0,0) + 70 alpha(0,1,0) + 100 alpha(0,2,0)
           + 50 alpha(0,3,0) + 20 alpha(0,4,0) <= 10
 cap(0,1): 50 alpha(0,0,1) + 70 alpha(0,1,1) + 100 alpha(0,2,1)
           + 50 alpha(0,3,1) + 20 alpha(0,4,1) <= 20
 cap(0,2): 50 alpha(0,0,2) + 70 alpha(0,1,2) + 100 alpha(0,2,2)
           + 50 alpha(0,3,2) + 20 alpha(0,4,2) <= 30
 cap(0,3): 50 alpha(0,0,3) + 70 alpha(0,1,3) + 100 alpha(0,2,3)
           + 50 alpha(0,3,3) + 20 alpha(0,4,3) <= 20
 cap(0,4): 50 alpha(0,0,4) + 70 alpha(0,1,4) + 100 alpha(0,2,4)
           + 50 alpha(0,3,4) + 20 alpha(0,4,4) <= 0
 cap(1,0): 50 alpha(1,0,0) + 70 alpha(1,1,0) + 100 alpha(1,2,0)
           + 50 alpha(1,3,0) + 20 alpha(1,4,0) <= 20
 cap(1,1): 50 alpha(1,0,1) + 70 alpha(1,1,1) + 100 alpha(1,2,1)
           + 50 alpha(1,3,1) + 20 alpha(1,4,1) <= 10
 cap(1,2): 50 alpha(1,0,2) + 70 alpha(1,1,2) + 100 alpha(1,2,2)
           + 50 alpha(1,3,2) + 20 alpha(1,4,2) <= 20
 cap(1,3): 50 alpha(1,0,3) + 70 alpha(1,1,3) + 100 alpha(1,2,3)
           + 50 alpha(1,3,3) + 20 alpha(1,4,3) <= 20
 cap(1,4): 50 alpha(1,0,4) + 70 alpha(1,1,4) + 100 alpha(1,2,4)
           + 50 alpha(1,3,4) + 20 alpha(1,4,4) <= 0
 cap(2,0): 50 alpha(2,0,0) + 70 alpha(2,1,0) + 100 alpha(2,2,0)
           + 50 alpha(2,3,0) + 20 alpha(2,4,0) <= 20
 cap(2,1): 50 alpha(2,0,1) + 70 alpha(2,1,1) + 100 alpha(2,2,1)
           + 50 alpha(2,3,1) + 20 alpha(2,4,1) <= 50
 cap(2,2): 50 alpha(2,0,2) + 70 alpha(2,1,2) + 100 alpha(2,2,2)
           + 50 alpha(2,3,2) + 20 alpha(2,4,2) <= 10
 cap(2,3): 50 alpha(2,0,3) + 70 alpha(2,1,3) + 100 alpha(2,2,3)
           + 50 alpha(2,3,3) + 20 alpha(2,4,3) <= 20
 cap(2,4): 50 alpha(2,0,4) + 70 alpha(2,1,4) + 100 alpha(2,2,4)
           + 50 alpha(2,3,4) + 20 alpha(2,4,4) <= 0
 cap(3,0): 50 alpha(3,0,0) + 70 alpha(3,1,0) + 100 alpha(3,2,0)
           + 50 alpha(3,3,0) + 20 alpha(3,4,0) <= 10
 cap(3,1): 50 alpha(3,0,1) + 70 alpha(3,1,1) + 100 alpha(3,2,1)
           + 50 alpha(3,3,1) + 20 alpha(3,4,1) <= 0
 cap(3,2): 50 alpha(3,0,2) + 70 alpha(3,1,2) + 100 alpha(3,2,2)
           + 50 alpha(3,3,2) + 20 alpha(3,4,2) <= 20
 cap(3,3): 50 alpha(3,0,3) + 70 alpha(3,1,3) + 100 alpha(3,2,3)
           + 50 alpha(3,3,3) + 20 alpha(3,4,3) <= 20
 cap(3,4): 50 alpha(3,0,4) + 70 alpha(3,1,4) + 100 alpha(3,2,4)
           + 50 alpha(3,3,4) + 20 alpha(3,4,4) <= 10
Bounds
 0 <= alpha(0,0,0) <= 1
 0 <= alpha(0,0,1) <= 1
 0 <= alpha(0,0,2) <= 1
 0 <= alpha(0,0,3) <= 1
 0 <= alpha(0,0,4) <= 1
 0 <= alpha(0,1,0) <= 1
 0 <= alpha(0,1,1) <= 1
 0 <= alpha(0,1,2) <= 1
 0 <= alpha(0,1,3) <= 1
 0 <= alpha(0,1,4) <= 1
 0 <= alpha(0,2,0) <= 1
 0 <= alpha(0,2,1) <= 1
 0 <= alpha(0,2,2) <= 1
 0 <= alpha(0,2,3) <= 1
 0 <= alpha(0,2,4) <= 1
 0 <= alpha(0,3,0) <= 1
 0 <= alpha(0,3,1) <= 1
 0 <= alpha(0,3,2) <= 1
 0 <= alpha(0,3,3) <= 1
 0 <= alpha(0,3,4) <= 1
 0 <= alpha(1,0,0) <= 1
 0 <= alpha(1,0,1) <= 1
 0 <= alpha(1,0,2) <= 1
 0 <= alpha(1,0,3) <= 1
 0 <= alpha(1,0,4) <= 1
 0 <= alpha(1,1,0) <= 1
 0 <= alpha(1,1,1) <= 1
 0 <= alpha(1,1,2) <= 1
 0 <= alpha(1,1,3) <= 1
 0 <= alpha(1,1,4) <= 1
 0 <= alpha(1,2,0) <= 1
 0 <= alpha(1,2,1) <= 1
 0 <= alpha(1,2,2) <= 1
 0 <= alpha(1,2,3) <= 1
 0 <= alpha(1,2,4) <= 1
 0 <= alpha(1,3,0) <= 1
 0 <= alpha(1,3,1) <= 1
 0 <= alpha(1,3,2) <= 1
 0 <= alpha(1,3,3) <= 1
 0 <= alpha(1,3,4) <= 1
 0 <= alpha(2,0,0) <= 1
 0 <= alpha(2,0,1) <= 1
 0 <= alpha(2,0,2) <= 1
 0 <= alpha(2,0,3) <= 1
 0 <= alpha(2,0,4) <= 1
 0 <= alpha(2,1,0) <= 1
 0 <= alpha(2,1,1) <= 1
 0 <= alpha(2,1,2) <= 1
 0 <= alpha(2,1,3) <= 1
 0 <= alpha(2,1,4) <= 1
 0 <= alpha(2,2,0) <= 1
 0 <= alpha(2,2,1) <= 1
 0 <= alpha(2,2,2) <= 1
 0 <= alpha(2,2,3) <= 1
 0 <= alpha(2,2,4) <= 1
 0 <= alpha(2,3,0) <= 1
 0 <= alpha(2,3,1) <= 1
 0 <= alpha(2,3,2) <= 1
 0 <= alpha(2,3,3) <= 1
 0 <= alpha(2,3,4) <= 1
 0 <= alpha(3,0,0) <= 1
 0 <= alpha(3,0,1) <= 1
 0 <= alpha(3,0,2) <= 1
 0 <= alpha(3,0,3) <= 1
 0 <= alpha(3,0,4) <= 1
 0 <= alpha(3,1,0) <= 1
 0 <= alpha(3,1,1) <= 1
 0 <= alpha(3,1,2) <= 1
 0 <= alpha(3,1,3) <= 1
 0 <= alpha(3,1,4) <= 1
 0 <= alpha(3,2,0) <= 1
 0 <= alpha(3,2,1) <= 1
 0 <= alpha(3,2,2) <= 1
 0 <= alpha(3,2,3) <= 1
 0 <= alpha(3,2,4) <= 1
 0 <= alpha(3,3,0) <= 1
 0 <= alpha(3,3,1) <= 1
 0 <= alpha(3,3,2) <= 1
 0 <= alpha(3,3,3) <= 1
 0 <= alpha(3,3,4) <= 1
 0 <= alpha(0,4,0) <= 1
 0 <= alpha(0,4,1) <= 1
 0 <= alpha(0,4,2) <= 1
 0 <= alpha(0,4,3) <= 1
 0 <= alpha(0,4,4) <= 1
 0 <= alpha(1,4,0) <= 1
 0 <= alpha(1,4,1) <= 1
 0 <= alpha(1,4,2) <= 1
 0 <= alpha(1,4,3) <= 1
 0 <= alpha(1,4,4) <= 1
 0 <= alpha(2,4,0) <= 1
 0 <= alpha(2,4,1) <= 1
 0 <= alpha(2,4,2) <= 1
 0 <= alpha(2,4,3) <= 1
 0 <= alpha(2,4,4) <= 1
 0 <= alpha(3,4,0) <= 1
 0 <= alpha(3,4,1) <= 1
 0 <= alpha(3,4,2) <= 1
 0 <= alpha(3,4,3) <= 1
 0 <= alpha(3,4,4) <= 1
End
