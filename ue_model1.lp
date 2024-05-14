\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 obj1: 500 alpha(0,0,0) + 3000 alpha(0,0,1) + 3000 alpha(0,0,2)
       + 3000 alpha(0,0,3) + 3000 alpha(0,0,4) + 4200 alpha(0,1,0)
       + 700 alpha(0,1,1) + 4200 alpha(0,1,2) + 4200 alpha(0,1,3)
       + 4200 alpha(0,1,4) + 6000 alpha(0,2,0) + 6000 alpha(0,2,1)
       + 1000 alpha(0,2,2) + 6000 alpha(0,2,3) + 6000 alpha(0,2,4)
       + 3000 alpha(0,3,0) + 3000 alpha(0,3,1) + 3000 alpha(0,3,2)
       + 500 alpha(0,3,3) + 3000 alpha(0,3,4) + 1200 alpha(0,4,0)
       + 1200 alpha(0,4,1) + 1200 alpha(0,4,2) + 1200 alpha(0,4,3)
       + 200 alpha(0,4,4) + 1500 alpha(1,0,0) + 4000 alpha(1,0,1)
       + 4000 alpha(1,0,2) + 4000 alpha(1,0,3) + 4000 alpha(1,0,4)
       + 5600 alpha(1,1,0) + 2100 alpha(1,1,1) + 5600 alpha(1,1,2)
       + 5600 alpha(1,1,3) + 5600 alpha(1,1,4) + 8000 alpha(1,2,0)
       + 8000 alpha(1,2,1) + 3000 alpha(1,2,2) + 8000 alpha(1,2,3)
       + 8000 alpha(1,2,4) + 4000 alpha(1,3,0) + 4000 alpha(1,3,1)
       + 4000 alpha(1,3,2) + 1500 alpha(1,3,3) + 4000 alpha(1,3,4)
       + 1600 alpha(1,4,0) + 1600 alpha(1,4,1) + 1600 alpha(1,4,2)
       + 1600 alpha(1,4,3) + 600 alpha(1,4,4) + 1000 alpha(2,0,0)
       + 3500 alpha(2,0,1) + 3500 alpha(2,0,2) + 3500 alpha(2,0,3)
       + 3500 alpha(2,0,4) + 4900 alpha(2,1,0) + 1400 alpha(2,1,1)
       + 4900 alpha(2,1,2) + 4900 alpha(2,1,3) + 4900 alpha(2,1,4)
       + 7000 alpha(2,2,0) + 7000 alpha(2,2,1) + 2000 alpha(2,2,2)
       + 7000 alpha(2,2,3) + 7000 alpha(2,2,4) + 3500 alpha(2,3,0)
       + 3500 alpha(2,3,1) + 3500 alpha(2,3,2) + 1000 alpha(2,3,3)
       + 3500 alpha(2,3,4) + 1400 alpha(2,4,0) + 1400 alpha(2,4,1)
       + 1400 alpha(2,4,2) + 1400 alpha(2,4,3) + 400 alpha(2,4,4)
       + 1000 alpha(3,0,0) + 3500 alpha(3,0,1) + 3500 alpha(3,0,2)
       + 3500 alpha(3,0,3) + 3500 alpha(3,0,4) + 4900 alpha(3,1,0)
       + 1400 alpha(3,1,1) + 4900 alpha(3,1,2) + 4900 alpha(3,1,3)
       + 4900 alpha(3,1,4) + 7000 alpha(3,2,0) + 7000 alpha(3,2,1)
       + 2000 alpha(3,2,2) + 7000 alpha(3,2,3) + 7000 alpha(3,2,4)
       + 3500 alpha(3,3,0) + 3500 alpha(3,3,1) + 3500 alpha(3,3,2)
       + 1000 alpha(3,3,3) + 3500 alpha(3,3,4) + 1400 alpha(3,4,0)
       + 1400 alpha(3,4,1) + 1400 alpha(3,4,2) + 1400 alpha(3,4,3)
       + 400 alpha(3,4,4)
Subject To
 con(0):       alpha(0,0,0) + alpha(0,0,1) + alpha(0,0,2) + alpha(0,0,3)
               + alpha(0,0,4) + alpha(1,0,0) + alpha(1,0,1) + alpha(1,0,2)
               + alpha(1,0,3) + alpha(1,0,4) + alpha(2,0,0) + alpha(2,0,1)
               + alpha(2,0,2) + alpha(2,0,3) + alpha(2,0,4) + alpha(3,0,0)
               + alpha(3,0,1) + alpha(3,0,2) + alpha(3,0,3) + alpha(3,0,4)  = 1
 con(1):       alpha(0,1,0) + alpha(0,1,1) + alpha(0,1,2) + alpha(0,1,3)
               + alpha(0,1,4) + alpha(1,1,0) + alpha(1,1,1) + alpha(1,1,2)
               + alpha(1,1,3) + alpha(1,1,4) + alpha(2,1,0) + alpha(2,1,1)
               + alpha(2,1,2) + alpha(2,1,3) + alpha(2,1,4) + alpha(3,1,0)
               + alpha(3,1,1) + alpha(3,1,2) + alpha(3,1,3) + alpha(3,1,4)  = 1
 con(2):       alpha(0,2,0) + alpha(0,2,1) + alpha(0,2,2) + alpha(0,2,3)
               + alpha(0,2,4) + alpha(1,2,0) + alpha(1,2,1) + alpha(1,2,2)
               + alpha(1,2,3) + alpha(1,2,4) + alpha(2,2,0) + alpha(2,2,1)
               + alpha(2,2,2) + alpha(2,2,3) + alpha(2,2,4) + alpha(3,2,0)
               + alpha(3,2,1) + alpha(3,2,2) + alpha(3,2,3) + alpha(3,2,4)  = 1
 con(3):       alpha(0,3,0) + alpha(0,3,1) + alpha(0,3,2) + alpha(0,3,3)
               + alpha(0,3,4) + alpha(1,3,0) + alpha(1,3,1) + alpha(1,3,2)
               + alpha(1,3,3) + alpha(1,3,4) + alpha(2,3,0) + alpha(2,3,1)
               + alpha(2,3,2) + alpha(2,3,3) + alpha(2,3,4) + alpha(3,3,0)
               + alpha(3,3,1) + alpha(3,3,2) + alpha(3,3,3) + alpha(3,3,4)  = 1
 con(4):       alpha(0,4,0) + alpha(0,4,1) + alpha(0,4,2) + alpha(0,4,3)
               + alpha(0,4,4) + alpha(1,4,0) + alpha(1,4,1) + alpha(1,4,2)
               + alpha(1,4,3) + alpha(1,4,4) + alpha(2,4,0) + alpha(2,4,1)
               + alpha(2,4,2) + alpha(2,4,3) + alpha(2,4,4) + alpha(3,4,0)
               + alpha(3,4,1) + alpha(3,4,2) + alpha(3,4,3) + alpha(3,4,4)  = 1
 cap(0,0):     50 alpha(0,0,0) + 70 alpha(0,1,0) + 100 alpha(0,2,0)
               + 50 alpha(0,3,0) + 20 alpha(0,4,0) <= 10
 cap(0,1):     50 alpha(0,0,1) + 70 alpha(0,1,1) + 100 alpha(0,2,1)
               + 50 alpha(0,3,1) + 20 alpha(0,4,1) <= 20
 cap(0,2):     50 alpha(0,0,2) + 70 alpha(0,1,2) + 100 alpha(0,2,2)
               + 50 alpha(0,3,2) + 20 alpha(0,4,2) <= 30
 cap(0,3):     50 alpha(0,0,3) + 70 alpha(0,1,3) + 100 alpha(0,2,3)
               + 50 alpha(0,3,3) + 20 alpha(0,4,3) <= 20
 cap(0,4):     50 alpha(0,0,4) + 70 alpha(0,1,4) + 100 alpha(0,2,4)
               + 50 alpha(0,3,4) + 20 alpha(0,4,4) <= 0
 cap(1,0):     50 alpha(1,0,0) + 70 alpha(1,1,0) + 100 alpha(1,2,0)
               + 50 alpha(1,3,0) + 20 alpha(1,4,0) <= 20
 cap(1,1):     50 alpha(1,0,1) + 70 alpha(1,1,1) + 100 alpha(1,2,1)
               + 50 alpha(1,3,1) + 20 alpha(1,4,1) <= 10
 cap(1,2):     50 alpha(1,0,2) + 70 alpha(1,1,2) + 100 alpha(1,2,2)
               + 50 alpha(1,3,2) + 20 alpha(1,4,2) <= 20
 cap(1,3):     50 alpha(1,0,3) + 70 alpha(1,1,3) + 100 alpha(1,2,3)
               + 50 alpha(1,3,3) + 20 alpha(1,4,3) <= 20
 cap(1,4):     50 alpha(1,0,4) + 70 alpha(1,1,4) + 100 alpha(1,2,4)
               + 50 alpha(1,3,4) + 20 alpha(1,4,4) <= 0
 cap(2,0):     50 alpha(2,0,0) + 70 alpha(2,1,0) + 100 alpha(2,2,0)
               + 50 alpha(2,3,0) + 20 alpha(2,4,0) <= 20
 cap(2,1):     50 alpha(2,0,1) + 70 alpha(2,1,1) + 100 alpha(2,2,1)
               + 50 alpha(2,3,1) + 20 alpha(2,4,1) <= 50
 cap(2,2):     50 alpha(2,0,2) + 70 alpha(2,1,2) + 100 alpha(2,2,2)
               + 50 alpha(2,3,2) + 20 alpha(2,4,2) <= 10
 cap(2,3):     50 alpha(2,0,3) + 70 alpha(2,1,3) + 100 alpha(2,2,3)
               + 50 alpha(2,3,3) + 20 alpha(2,4,3) <= 20
 cap(2,4):     50 alpha(2,0,4) + 70 alpha(2,1,4) + 100 alpha(2,2,4)
               + 50 alpha(2,3,4) + 20 alpha(2,4,4) <= 0
 cap(3,0):     50 alpha(3,0,0) + 70 alpha(3,1,0) + 100 alpha(3,2,0)
               + 50 alpha(3,3,0) + 20 alpha(3,4,0) <= 10
 cap(3,1):     50 alpha(3,0,1) + 70 alpha(3,1,1) + 100 alpha(3,2,1)
               + 50 alpha(3,3,1) + 20 alpha(3,4,1) <= 0
 cap(3,2):     50 alpha(3,0,2) + 70 alpha(3,1,2) + 100 alpha(3,2,2)
               + 50 alpha(3,3,2) + 20 alpha(3,4,2) <= 20
 cap(3,3):     50 alpha(3,0,3) + 70 alpha(3,1,3) + 100 alpha(3,2,3)
               + 50 alpha(3,3,3) + 20 alpha(3,4,3) <= 20
 cap(3,4):     50 alpha(3,0,4) + 70 alpha(3,1,4) + 100 alpha(3,2,4)
               + 50 alpha(3,3,4) + 20 alpha(3,4,4) <= 10
 alpha_r(0):   alpha(0,0,1) + alpha(0,0,2) + alpha(0,0,3) + alpha(0,0,4)
               + alpha(1,0,1) + alpha(1,0,2) + alpha(1,0,3) + alpha(1,0,4)
               + alpha(2,0,1) + alpha(2,0,2) + alpha(2,0,3) + alpha(2,0,4)
               + alpha(3,0,1) + alpha(3,0,2) + alpha(3,0,3) + alpha(3,0,4)
               - r(0) <= 0
 alpha_r(1):   alpha(0,1,0) + alpha(0,1,2) + alpha(0,1,3) + alpha(0,1,4)
               + alpha(1,1,0) + alpha(1,1,2) + alpha(1,1,3) + alpha(1,1,4)
               + alpha(2,1,0) + alpha(2,1,2) + alpha(2,1,3) + alpha(2,1,4)
               + alpha(3,1,0) + alpha(3,1,2) + alpha(3,1,3) + alpha(3,1,4)
               - r(1) <= 0
 alpha_r(2):   alpha(0,2,0) + alpha(0,2,1) + alpha(0,2,3) + alpha(0,2,4)
               + alpha(1,2,0) + alpha(1,2,1) + alpha(1,2,3) + alpha(1,2,4)
               + alpha(2,2,0) + alpha(2,2,1) + alpha(2,2,3) + alpha(2,2,4)
               + alpha(3,2,0) + alpha(3,2,1) + alpha(3,2,3) + alpha(3,2,4)
               - r(2) <= 0
 alpha_r(3):   alpha(0,3,0) + alpha(0,3,1) + alpha(0,3,2) + alpha(0,3,4)
               + alpha(1,3,0) + alpha(1,3,1) + alpha(1,3,2) + alpha(1,3,4)
               + alpha(2,3,0) + alpha(2,3,1) + alpha(2,3,2) + alpha(2,3,4)
               + alpha(3,3,0) + alpha(3,3,1) + alpha(3,3,2) + alpha(3,3,4)
               - r(3) <= 0
 alpha_r(4):   alpha(0,4,0) + alpha(0,4,1) + alpha(0,4,2) + alpha(0,4,3)
               + alpha(1,4,0) + alpha(1,4,1) + alpha(1,4,2) + alpha(1,4,3)
               + alpha(2,4,0) + alpha(2,4,1) + alpha(2,4,2) + alpha(2,4,3)
               + alpha(3,4,0) + alpha(3,4,1) + alpha(3,4,2) + alpha(3,4,3)
               - r(4) <= 0
 r_alpha(0):   - 50 alpha(0,0,0) - 70 alpha(0,1,0) - 100 alpha(0,2,0)
               - 50 alpha(0,3,0) - 20 alpha(0,4,0) - 50 alpha(1,0,0)
               - 70 alpha(1,1,0) - 100 alpha(1,2,0) - 50 alpha(1,3,0)
               - 20 alpha(1,4,0) - 50 alpha(2,0,0) - 70 alpha(2,1,0)
               - 100 alpha(2,2,0) - 50 alpha(2,3,0) - 20 alpha(2,4,0)
               - 50 alpha(3,0,0) - 70 alpha(3,1,0) - 100 alpha(3,2,0)
               - 50 alpha(3,3,0) - 20 alpha(3,4,0) + 60 r(0) <= 0
 r_alpha(1):   - 50 alpha(0,0,1) - 70 alpha(0,1,1) - 100 alpha(0,2,1)
               - 50 alpha(0,3,1) - 20 alpha(0,4,1) - 50 alpha(1,0,1)
               - 70 alpha(1,1,1) - 100 alpha(1,2,1) - 50 alpha(1,3,1)
               - 20 alpha(1,4,1) - 50 alpha(2,0,1) - 70 alpha(2,1,1)
               - 100 alpha(2,2,1) - 50 alpha(2,3,1) - 20 alpha(2,4,1)
               - 50 alpha(3,0,1) - 70 alpha(3,1,1) - 100 alpha(3,2,1)
               - 50 alpha(3,3,1) - 20 alpha(3,4,1) + 80 r(1) <= 0
 r_alpha(2):   - 50 alpha(0,0,2) - 70 alpha(0,1,2) - 100 alpha(0,2,2)
               - 50 alpha(0,3,2) - 20 alpha(0,4,2) - 50 alpha(1,0,2)
               - 70 alpha(1,1,2) - 100 alpha(1,2,2) - 50 alpha(1,3,2)
               - 20 alpha(1,4,2) - 50 alpha(2,0,2) - 70 alpha(2,1,2)
               - 100 alpha(2,2,2) - 50 alpha(2,3,2) - 20 alpha(2,4,2)
               - 50 alpha(3,0,2) - 70 alpha(3,1,2) - 100 alpha(3,2,2)
               - 50 alpha(3,3,2) - 20 alpha(3,4,2) + 80 r(2) <= 0
 r_alpha(3):   - 50 alpha(0,0,3) - 70 alpha(0,1,3) - 100 alpha(0,2,3)
               - 50 alpha(0,3,3) - 20 alpha(0,4,3) - 50 alpha(1,0,3)
               - 70 alpha(1,1,3) - 100 alpha(1,2,3) - 50 alpha(1,3,3)
               - 20 alpha(1,4,3) - 50 alpha(2,0,3) - 70 alpha(2,1,3)
               - 100 alpha(2,2,3) - 50 alpha(2,3,3) - 20 alpha(2,4,3)
               - 50 alpha(3,0,3) - 70 alpha(3,1,3) - 100 alpha(3,2,3)
               - 50 alpha(3,3,3) - 20 alpha(3,4,3) + 80 r(3) <= 0
 r_alpha(4):   - 50 alpha(0,0,4) - 70 alpha(0,1,4) - 100 alpha(0,2,4)
               - 50 alpha(0,3,4) - 20 alpha(0,4,4) - 50 alpha(1,0,4)
               - 70 alpha(1,1,4) - 100 alpha(1,2,4) - 50 alpha(1,3,4)
               - 20 alpha(1,4,4) - 50 alpha(2,0,4) - 70 alpha(2,1,4)
               - 100 alpha(2,2,4) - 50 alpha(2,3,4) - 20 alpha(2,4,4)
               - 50 alpha(3,0,4) - 70 alpha(3,1,4) - 100 alpha(3,2,4)
               - 50 alpha(3,3,4) - 20 alpha(3,4,4) + 10 r(4) <= 0
 u_v(0,1):     u(0) - u(1) + 5 v(0,1) <= 4
 alpha_v(0,1): alpha(0,0,1) + alpha(1,0,1) + alpha(2,0,1) + alpha(3,0,1)
               - v(0,1) <= 0
 u_v(0,2):     u(0) - u(2) + 5 v(0,2) <= 4
 alpha_v(0,2): alpha(0,0,2) + alpha(1,0,2) + alpha(2,0,2) + alpha(3,0,2)
               - v(0,2) <= 0
 u_v(0,3):     u(0) - u(3) + 5 v(0,3) <= 4
 alpha_v(0,3): alpha(0,0,3) + alpha(1,0,3) + alpha(2,0,3) + alpha(3,0,3)
               - v(0,3) <= 0
 u_v(0,4):     u(0) - u(4) + 5 v(0,4) <= 4
 alpha_v(0,4): alpha(0,0,4) + alpha(1,0,4) + alpha(2,0,4) + alpha(3,0,4)
               - v(0,4) <= 0
 u_v(1,0):     - u(0) + u(1) + 5 v(1,0) <= 4
 alpha_v(1,0): alpha(0,1,0) + alpha(1,1,0) + alpha(2,1,0) + alpha(3,1,0)
               - v(1,0) <= 0
 u_v(1,2):     u(1) - u(2) + 5 v(1,2) <= 4
 alpha_v(1,2): alpha(0,1,2) + alpha(1,1,2) + alpha(2,1,2) + alpha(3,1,2)
               - v(1,2) <= 0
 u_v(1,3):     u(1) - u(3) + 5 v(1,3) <= 4
 alpha_v(1,3): alpha(0,1,3) + alpha(1,1,3) + alpha(2,1,3) + alpha(3,1,3)
               - v(1,3) <= 0
 u_v(1,4):     u(1) - u(4) + 5 v(1,4) <= 4
 alpha_v(1,4): alpha(0,1,4) + alpha(1,1,4) + alpha(2,1,4) + alpha(3,1,4)
               - v(1,4) <= 0
 u_v(2,0):     - u(0) + u(2) + 5 v(2,0) <= 4
 alpha_v(2,0): alpha(0,2,0) + alpha(1,2,0) + alpha(2,2,0) + alpha(3,2,0)
               - v(2,0) <= 0
 u_v(2,1):     - u(1) + u(2) + 5 v(2,1) <= 4
 alpha_v(2,1): alpha(0,2,1) + alpha(1,2,1) + alpha(2,2,1) + alpha(3,2,1)
               - v(2,1) <= 0
 u_v(2,3):     u(2) - u(3) + 5 v(2,3) <= 4
 alpha_v(2,3): alpha(0,2,3) + alpha(1,2,3) + alpha(2,2,3) + alpha(3,2,3)
               - v(2,3) <= 0
 u_v(2,4):     u(2) - u(4) + 5 v(2,4) <= 4
 alpha_v(2,4): alpha(0,2,4) + alpha(1,2,4) + alpha(2,2,4) + alpha(3,2,4)
               - v(2,4) <= 0
 u_v(3,0):     - u(0) + u(3) + 5 v(3,0) <= 4
 alpha_v(3,0): alpha(0,3,0) + alpha(1,3,0) + alpha(2,3,0) + alpha(3,3,0)
               - v(3,0) <= 0
 u_v(3,1):     - u(1) + u(3) + 5 v(3,1) <= 4
 alpha_v(3,1): alpha(0,3,1) + alpha(1,3,1) + alpha(2,3,1) + alpha(3,3,1)
               - v(3,1) <= 0
 u_v(3,2):     - u(2) + u(3) + 5 v(3,2) <= 4
 alpha_v(3,2): alpha(0,3,2) + alpha(1,3,2) + alpha(2,3,2) + alpha(3,3,2)
               - v(3,2) <= 0
 u_v(3,4):     u(3) - u(4) + 5 v(3,4) <= 4
 alpha_v(3,4): alpha(0,3,4) + alpha(1,3,4) + alpha(2,3,4) + alpha(3,3,4)
               - v(3,4) <= 0
 u_v(4,0):     - u(0) + u(4) + 5 v(4,0) <= 4
 alpha_v(4,0): alpha(0,4,0) + alpha(1,4,0) + alpha(2,4,0) + alpha(3,4,0)
               - v(4,0) <= 0
 u_v(4,1):     - u(1) + u(4) + 5 v(4,1) <= 4
 alpha_v(4,1): alpha(0,4,1) + alpha(1,4,1) + alpha(2,4,1) + alpha(3,4,1)
               - v(4,1) <= 0
 u_v(4,2):     - u(2) + u(4) + 5 v(4,2) <= 4
 alpha_v(4,2): alpha(0,4,2) + alpha(1,4,2) + alpha(2,4,2) + alpha(3,4,2)
               - v(4,2) <= 0
 u_v(4,3):     - u(3) + u(4) + 5 v(4,3) <= 4
 alpha_v(4,3): alpha(0,4,3) + alpha(1,4,3) + alpha(2,4,3) + alpha(3,4,3)
               - v(4,3) <= 0
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
 0 <= alpha(0,4,0) <= 1
 0 <= alpha(0,4,1) <= 1
 0 <= alpha(0,4,2) <= 1
 0 <= alpha(0,4,3) <= 1
 0 <= alpha(0,4,4) <= 1
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
 0 <= alpha(1,4,0) <= 1
 0 <= alpha(1,4,1) <= 1
 0 <= alpha(1,4,2) <= 1
 0 <= alpha(1,4,3) <= 1
 0 <= alpha(1,4,4) <= 1
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
 0 <= alpha(2,4,0) <= 1
 0 <= alpha(2,4,1) <= 1
 0 <= alpha(2,4,2) <= 1
 0 <= alpha(2,4,3) <= 1
 0 <= alpha(2,4,4) <= 1
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
 0 <= alpha(3,4,0) <= 1
 0 <= alpha(3,4,1) <= 1
 0 <= alpha(3,4,2) <= 1
 0 <= alpha(3,4,3) <= 1
 0 <= alpha(3,4,4) <= 1
 0 <= r(0) <= 1
 0 <= r(1) <= 1
 0 <= r(2) <= 1
 0 <= r(3) <= 1
 0 <= r(4) <= 1
 0 <= u(0) <= 5
 0 <= u(1) <= 5
 0 <= v(0,1) <= 1
 0 <= u(2) <= 5
 0 <= v(0,2) <= 1
 0 <= u(3) <= 5
 0 <= v(0,3) <= 1
 0 <= u(4) <= 5
 0 <= v(0,4) <= 1
 0 <= v(1,0) <= 1
 0 <= v(1,2) <= 1
 0 <= v(1,3) <= 1
 0 <= v(1,4) <= 1
 0 <= v(2,0) <= 1
 0 <= v(2,1) <= 1
 0 <= v(2,3) <= 1
 0 <= v(2,4) <= 1
 0 <= v(3,0) <= 1
 0 <= v(3,1) <= 1
 0 <= v(3,2) <= 1
 0 <= v(3,4) <= 1
 0 <= v(4,0) <= 1
 0 <= v(4,1) <= 1
 0 <= v(4,2) <= 1
 0 <= v(4,3) <= 1
Binaries
 r(0)  r(1)  r(2)  r(3)  r(4)  v(0,1)  v(0,2)  v(0,3)  v(0,4)  v(1,0)  v(1,2) 
 v(1,3)  v(1,4)  v(2,0)  v(2,1)  v(2,3)  v(2,4)  v(3,0)  v(3,1)  v(3,2) 
 v(3,4)  v(4,0)  v(4,1)  v(4,2)  v(4,3) 
End
