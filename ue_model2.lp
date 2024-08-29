\ENCODING=ISO-8859-1
\Problem name: IloCplex

Maximize
 #0: 5415 alpha(0,0,0) + 5415 alpha(0,0,1) + 5415 alpha(0,0,2)
     + 5415 alpha(0,0,3) + 1976 alpha(0,1,0) + 1976 alpha(0,1,1)
     + 1976 alpha(0,1,2) + 1976 alpha(0,1,3) + 3363 alpha(0,2,0)
     + 3363 alpha(0,2,1) + 3363 alpha(0,2,2) + 3363 alpha(0,2,3)
     + 741 alpha(0,3,0) + 741 alpha(0,3,1) + 741 alpha(0,3,2)
     + 741 alpha(0,3,3) + 3135 alpha(1,0,0) + 3135 alpha(1,0,1)
     + 3135 alpha(1,0,2) + 3135 alpha(1,0,3) + 1144 alpha(1,1,0)
     + 1144 alpha(1,1,1) + 1144 alpha(1,1,2) + 1144 alpha(1,1,3)
     + 1947 alpha(1,2,0) + 1947 alpha(1,2,1) + 1947 alpha(1,2,2)
     + 1947 alpha(1,2,3) + 429 alpha(1,3,0) + 429 alpha(1,3,1)
     + 429 alpha(1,3,2) + 429 alpha(1,3,3) + 3135 alpha(2,0,0)
     + 3135 alpha(2,0,1) + 3135 alpha(2,0,2) + 3135 alpha(2,0,3)
     + 1144 alpha(2,1,0) + 1144 alpha(2,1,1) + 1144 alpha(2,1,2)
     + 1144 alpha(2,1,3) + 1947 alpha(2,2,0) + 1947 alpha(2,2,1)
     + 1947 alpha(2,2,2) + 1947 alpha(2,2,3) + 429 alpha(2,3,0)
     + 429 alpha(2,3,1) + 429 alpha(2,3,2) + 429 alpha(2,3,3)
     + 10260 alpha(3,0,0) + 10260 alpha(3,0,1) + 10260 alpha(3,0,2)
     + 10260 alpha(3,0,3) + 3744 alpha(3,1,0) + 3744 alpha(3,1,1)
     + 3744 alpha(3,1,2) + 3744 alpha(3,1,3) + 6372 alpha(3,2,0)
     + 6372 alpha(3,2,1) + 6372 alpha(3,2,2) + 6372 alpha(3,2,3)
     + 1404 alpha(3,3,0) + 1404 alpha(3,3,1) + 1404 alpha(3,3,2)
     + 1404 alpha(3,3,3) + 9405 alpha(4,0,0) + 9405 alpha(4,0,1)
     + 9405 alpha(4,0,2) + 9405 alpha(4,0,3) + 3432 alpha(4,1,0)
     + 3432 alpha(4,1,1) + 3432 alpha(4,1,2) + 3432 alpha(4,1,3)
     + 5841 alpha(4,2,0) + 5841 alpha(4,2,1) + 5841 alpha(4,2,2)
     + 5841 alpha(4,2,3) + 1287 alpha(4,3,0) + 1287 alpha(4,3,1)
     + 1287 alpha(4,3,2) + 1287 alpha(4,3,3)
Subject To
 con(0):       alpha(0,0,0) + alpha(0,0,1) + alpha(0,0,2) + alpha(0,0,3)
               + alpha(1,0,0) + alpha(1,0,1) + alpha(1,0,2) + alpha(1,0,3)
               + alpha(2,0,0) + alpha(2,0,1) + alpha(2,0,2) + alpha(2,0,3)
               + alpha(3,0,0) + alpha(3,0,1) + alpha(3,0,2) + alpha(3,0,3)
               + alpha(4,0,0) + alpha(4,0,1) + alpha(4,0,2) + alpha(4,0,3)  = 1
 con(1):       alpha(0,1,0) + alpha(0,1,1) + alpha(0,1,2) + alpha(0,1,3)
               + alpha(1,1,0) + alpha(1,1,1) + alpha(1,1,2) + alpha(1,1,3)
               + alpha(2,1,0) + alpha(2,1,1) + alpha(2,1,2) + alpha(2,1,3)
               + alpha(3,1,0) + alpha(3,1,1) + alpha(3,1,2) + alpha(3,1,3)
               + alpha(4,1,0) + alpha(4,1,1) + alpha(4,1,2) + alpha(4,1,3)  = 1
 con(2):       alpha(0,2,0) + alpha(0,2,1) + alpha(0,2,2) + alpha(0,2,3)
               + alpha(1,2,0) + alpha(1,2,1) + alpha(1,2,2) + alpha(1,2,3)
               + alpha(2,2,0) + alpha(2,2,1) + alpha(2,2,2) + alpha(2,2,3)
               + alpha(3,2,0) + alpha(3,2,1) + alpha(3,2,2) + alpha(3,2,3)
               + alpha(4,2,0) + alpha(4,2,1) + alpha(4,2,2) + alpha(4,2,3)  = 1
 con(3):       alpha(0,3,0) + alpha(0,3,1) + alpha(0,3,2) + alpha(0,3,3)
               + alpha(1,3,0) + alpha(1,3,1) + alpha(1,3,2) + alpha(1,3,3)
               + alpha(2,3,0) + alpha(2,3,1) + alpha(2,3,2) + alpha(2,3,3)
               + alpha(3,3,0) + alpha(3,3,1) + alpha(3,3,2) + alpha(3,3,3)
               + alpha(4,3,0) + alpha(4,3,1) + alpha(4,3,2) + alpha(4,3,3)  = 1
 cap(0,0):     285 alpha(0,0,0) + 104 alpha(0,1,0) + 177 alpha(0,2,0)
               + 39 alpha(0,3,0) <= 12
 cap(0,1):     285 alpha(0,0,1) + 104 alpha(0,1,1) + 177 alpha(0,2,1)
               + 39 alpha(0,3,1) <= 52
 cap(0,2):     285 alpha(0,0,2) + 104 alpha(0,1,2) + 177 alpha(0,2,2)
               + 39 alpha(0,3,2) <= 99
 cap(0,3):     285 alpha(0,0,3) + 104 alpha(0,1,3) + 177 alpha(0,2,3)
               + 39 alpha(0,3,3) <= 48
 cap(1,0):     285 alpha(1,0,0) + 104 alpha(1,1,0) + 177 alpha(1,2,0)
               + 39 alpha(1,3,0) <= 57
 cap(1,1):     285 alpha(1,0,1) + 104 alpha(1,1,1) + 177 alpha(1,2,1)
               + 39 alpha(1,3,1) <= 35
 cap(1,2):     285 alpha(1,0,2) + 104 alpha(1,1,2) + 177 alpha(1,2,2)
               + 39 alpha(1,3,2) <= 57
 cap(1,3):     285 alpha(1,0,3) + 104 alpha(1,1,3) + 177 alpha(1,2,3)
               + 39 alpha(1,3,3) <= 90
 cap(2,0):     285 alpha(2,0,0) + 104 alpha(2,1,0) + 177 alpha(2,2,0)
               + 39 alpha(2,3,0) <= 37
 cap(2,1):     285 alpha(2,0,1) + 104 alpha(2,1,1) + 177 alpha(2,2,1)
               + 39 alpha(2,3,1) <= 57
 cap(2,2):     285 alpha(2,0,2) + 104 alpha(2,1,2) + 177 alpha(2,2,2)
               + 39 alpha(2,3,2) <= 95
 cap(2,3):     285 alpha(2,0,3) + 104 alpha(2,1,3) + 177 alpha(2,2,3)
               + 39 alpha(2,3,3) <= 14
 cap(3,0):     285 alpha(3,0,0) + 104 alpha(3,1,0) + 177 alpha(3,2,0)
               + 39 alpha(3,3,0) <= 52
 cap(3,1):     285 alpha(3,0,1) + 104 alpha(3,1,1) + 177 alpha(3,2,1)
               + 39 alpha(3,3,1) <= 46
 cap(3,2):     285 alpha(3,0,2) + 104 alpha(3,1,2) + 177 alpha(3,2,2)
               + 39 alpha(3,3,2) <= 52
 cap(3,3):     285 alpha(3,0,3) + 104 alpha(3,1,3) + 177 alpha(3,2,3)
               + 39 alpha(3,3,3) <= 89
 cap(4,0):     285 alpha(4,0,0) + 104 alpha(4,1,0) + 177 alpha(4,2,0)
               + 39 alpha(4,3,0) <= 49
 cap(4,1):     285 alpha(4,0,1) + 104 alpha(4,1,1) + 177 alpha(4,2,1)
               + 39 alpha(4,3,1) <= 75
 cap(4,2):     285 alpha(4,0,2) + 104 alpha(4,1,2) + 177 alpha(4,2,2)
               + 39 alpha(4,3,2) <= 100
 cap(4,3):     285 alpha(4,0,3) + 104 alpha(4,1,3) + 177 alpha(4,2,3)
               + 39 alpha(4,3,3) <= 46
 alpha_r(0):   alpha(0,0,1) + alpha(0,0,2) + alpha(0,0,3) + alpha(1,0,1)
               + alpha(1,0,2) + alpha(1,0,3) + alpha(2,0,1) + alpha(2,0,2)
               + alpha(2,0,3) + alpha(3,0,1) + alpha(3,0,2) + alpha(3,0,3)
               + alpha(4,0,1) + alpha(4,0,2) + alpha(4,0,3) - r(0) <= 0
 alpha_r(1):   alpha(0,1,0) + alpha(0,1,2) + alpha(0,1,3) + alpha(1,1,0)
               + alpha(1,1,2) + alpha(1,1,3) + alpha(2,1,0) + alpha(2,1,2)
               + alpha(2,1,3) + alpha(3,1,0) + alpha(3,1,2) + alpha(3,1,3)
               + alpha(4,1,0) + alpha(4,1,2) + alpha(4,1,3) - r(1) <= 0
 alpha_r(2):   alpha(0,2,0) + alpha(0,2,1) + alpha(0,2,3) + alpha(1,2,0)
               + alpha(1,2,1) + alpha(1,2,3) + alpha(2,2,0) + alpha(2,2,1)
               + alpha(2,2,3) + alpha(3,2,0) + alpha(3,2,1) + alpha(3,2,3)
               + alpha(4,2,0) + alpha(4,2,1) + alpha(4,2,3) - r(2) <= 0
 alpha_r(3):   alpha(0,3,0) + alpha(0,3,1) + alpha(0,3,2) + alpha(1,3,0)
               + alpha(1,3,1) + alpha(1,3,2) + alpha(2,3,0) + alpha(2,3,1)
               + alpha(2,3,2) + alpha(3,3,0) + alpha(3,3,1) + alpha(3,3,2)
               + alpha(4,3,0) + alpha(4,3,1) + alpha(4,3,2) - r(3) <= 0
 r_alpha(0):   - 285 alpha(0,0,0) - 104 alpha(0,1,0) - 177 alpha(0,2,0)
               - 39 alpha(0,3,0) - 285 alpha(1,0,0) - 104 alpha(1,1,0)
               - 177 alpha(1,2,0) - 39 alpha(1,3,0) - 285 alpha(2,0,0)
               - 104 alpha(2,1,0) - 177 alpha(2,2,0) - 39 alpha(2,3,0)
               - 285 alpha(3,0,0) - 104 alpha(3,1,0) - 177 alpha(3,2,0)
               - 39 alpha(3,3,0) - 285 alpha(4,0,0) - 104 alpha(4,1,0)
               - 177 alpha(4,2,0) - 39 alpha(4,3,0) + 207 r(0) <= 0
 r_alpha(1):   - 285 alpha(0,0,1) - 104 alpha(0,1,1) - 177 alpha(0,2,1)
               - 39 alpha(0,3,1) - 285 alpha(1,0,1) - 104 alpha(1,1,1)
               - 177 alpha(1,2,1) - 39 alpha(1,3,1) - 285 alpha(2,0,1)
               - 104 alpha(2,1,1) - 177 alpha(2,2,1) - 39 alpha(2,3,1)
               - 285 alpha(3,0,1) - 104 alpha(3,1,1) - 177 alpha(3,2,1)
               - 39 alpha(3,3,1) - 285 alpha(4,0,1) - 104 alpha(4,1,1)
               - 177 alpha(4,2,1) - 39 alpha(4,3,1) + 265 r(1) <= 0
 r_alpha(2):   - 285 alpha(0,0,2) - 104 alpha(0,1,2) - 177 alpha(0,2,2)
               - 39 alpha(0,3,2) - 285 alpha(1,0,2) - 104 alpha(1,1,2)
               - 177 alpha(1,2,2) - 39 alpha(1,3,2) - 285 alpha(2,0,2)
               - 104 alpha(2,1,2) - 177 alpha(2,2,2) - 39 alpha(2,3,2)
               - 285 alpha(3,0,2) - 104 alpha(3,1,2) - 177 alpha(3,2,2)
               - 39 alpha(3,3,2) - 285 alpha(4,0,2) - 104 alpha(4,1,2)
               - 177 alpha(4,2,2) - 39 alpha(4,3,2) + 403 r(2) <= 0
 r_alpha(3):   - 285 alpha(0,0,3) - 104 alpha(0,1,3) - 177 alpha(0,2,3)
               - 39 alpha(0,3,3) - 285 alpha(1,0,3) - 104 alpha(1,1,3)
               - 177 alpha(1,2,3) - 39 alpha(1,3,3) - 285 alpha(2,0,3)
               - 104 alpha(2,1,3) - 177 alpha(2,2,3) - 39 alpha(2,3,3)
               - 285 alpha(3,0,3) - 104 alpha(3,1,3) - 177 alpha(3,2,3)
               - 39 alpha(3,3,3) - 285 alpha(4,0,3) - 104 alpha(4,1,3)
               - 177 alpha(4,2,3) - 39 alpha(4,3,3) + 287 r(3) <= 0
 u_v(0,1):     u(0) - u(1) + 4 v(0,1) <= 3
 alpha_v(0,1): alpha(0,0,1) + alpha(1,0,1) + alpha(2,0,1) + alpha(3,0,1)
               + alpha(4,0,1) - v(0,1) <= 0
 u_v(0,2):     u(0) - u(2) + 4 v(0,2) <= 3
 alpha_v(0,2): alpha(0,0,2) + alpha(1,0,2) + alpha(2,0,2) + alpha(3,0,2)
               + alpha(4,0,2) - v(0,2) <= 0
 u_v(0,3):     u(0) - u(3) + 4 v(0,3) <= 3
 alpha_v(0,3): alpha(0,0,3) + alpha(1,0,3) + alpha(2,0,3) + alpha(3,0,3)
               + alpha(4,0,3) - v(0,3) <= 0
 u_v(1,0):     - u(0) + u(1) + 4 v(1,0) <= 3
 alpha_v(1,0): alpha(0,1,0) + alpha(1,1,0) + alpha(2,1,0) + alpha(3,1,0)
               + alpha(4,1,0) - v(1,0) <= 0
 u_v(1,2):     u(1) - u(2) + 4 v(1,2) <= 3
 alpha_v(1,2): alpha(0,1,2) + alpha(1,1,2) + alpha(2,1,2) + alpha(3,1,2)
               + alpha(4,1,2) - v(1,2) <= 0
 u_v(1,3):     u(1) - u(3) + 4 v(1,3) <= 3
 alpha_v(1,3): alpha(0,1,3) + alpha(1,1,3) + alpha(2,1,3) + alpha(3,1,3)
               + alpha(4,1,3) - v(1,3) <= 0
 u_v(2,0):     - u(0) + u(2) + 4 v(2,0) <= 3
 alpha_v(2,0): alpha(0,2,0) + alpha(1,2,0) + alpha(2,2,0) + alpha(3,2,0)
               + alpha(4,2,0) - v(2,0) <= 0
 u_v(2,1):     - u(1) + u(2) + 4 v(2,1) <= 3
 alpha_v(2,1): alpha(0,2,1) + alpha(1,2,1) + alpha(2,2,1) + alpha(3,2,1)
               + alpha(4,2,1) - v(2,1) <= 0
 u_v(2,3):     u(2) - u(3) + 4 v(2,3) <= 3
 alpha_v(2,3): alpha(0,2,3) + alpha(1,2,3) + alpha(2,2,3) + alpha(3,2,3)
               + alpha(4,2,3) - v(2,3) <= 0
 u_v(3,0):     - u(0) + u(3) + 4 v(3,0) <= 3
 alpha_v(3,0): alpha(0,3,0) + alpha(1,3,0) + alpha(2,3,0) + alpha(3,3,0)
               + alpha(4,3,0) - v(3,0) <= 0
 u_v(3,1):     - u(1) + u(3) + 4 v(3,1) <= 3
 alpha_v(3,1): alpha(0,3,1) + alpha(1,3,1) + alpha(2,3,1) + alpha(3,3,1)
               + alpha(4,3,1) - v(3,1) <= 0
 u_v(3,2):     - u(2) + u(3) + 4 v(3,2) <= 3
 alpha_v(3,2): alpha(0,3,2) + alpha(1,3,2) + alpha(2,3,2) + alpha(3,3,2)
               + alpha(4,3,2) - v(3,2) <= 0
 c57:          285 alpha(0,0,1) + 285 alpha(0,0,2) + 285 alpha(0,0,3)
               + 104 alpha(0,1,0) + 104 alpha(0,1,2) + 104 alpha(0,1,3)
               + 177 alpha(0,2,0) + 177 alpha(0,2,1) + 177 alpha(0,2,3)
               + 39 alpha(0,3,0) + 39 alpha(0,3,1) + 39 alpha(0,3,2)
               + 285 alpha(1,0,1) + 285 alpha(1,0,2) + 285 alpha(1,0,3)
               + 104 alpha(1,1,0) + 104 alpha(1,1,2) + 104 alpha(1,1,3)
               + 177 alpha(1,2,0) + 177 alpha(1,2,1) + 177 alpha(1,2,3)
               + 39 alpha(1,3,0) + 39 alpha(1,3,1) + 39 alpha(1,3,2)
               + 285 alpha(2,0,1) + 285 alpha(2,0,2) + 285 alpha(2,0,3)
               + 104 alpha(2,1,0) + 104 alpha(2,1,2) + 104 alpha(2,1,3)
               + 177 alpha(2,2,0) + 177 alpha(2,2,1) + 177 alpha(2,2,3)
               + 39 alpha(2,3,0) + 39 alpha(2,3,1) + 39 alpha(2,3,2)
               + 285 alpha(3,0,1) + 285 alpha(3,0,2) + 285 alpha(3,0,3)
               + 104 alpha(3,1,0) + 104 alpha(3,1,2) + 104 alpha(3,1,3)
               + 177 alpha(3,2,0) + 177 alpha(3,2,1) + 177 alpha(3,2,3)
               + 39 alpha(3,3,0) + 39 alpha(3,3,1) + 39 alpha(3,3,2)
               + 285 alpha(4,0,1) + 285 alpha(4,0,2) + 285 alpha(4,0,3)
               + 104 alpha(4,1,0) + 104 alpha(4,1,2) + 104 alpha(4,1,3)
               + 177 alpha(4,2,0) + 177 alpha(4,2,1) + 177 alpha(4,2,3)
               + 39 alpha(4,3,0) + 39 alpha(4,3,1) + 39 alpha(4,3,2) >= 78
Bounds
 0 <= alpha(0,0,0) <= 1
 0 <= alpha(0,0,1) <= 1
 0 <= alpha(0,0,2) <= 1
 0 <= alpha(0,0,3) <= 1
 0 <= alpha(0,1,0) <= 1
 0 <= alpha(0,1,1) <= 1
 0 <= alpha(0,1,2) <= 1
 0 <= alpha(0,1,3) <= 1
 0 <= alpha(0,2,0) <= 1
 0 <= alpha(0,2,1) <= 1
 0 <= alpha(0,2,2) <= 1
 0 <= alpha(0,2,3) <= 1
 0 <= alpha(0,3,0) <= 1
 0 <= alpha(0,3,1) <= 1
 0 <= alpha(0,3,2) <= 1
 0 <= alpha(0,3,3) <= 1
 0 <= alpha(1,0,0) <= 1
 0 <= alpha(1,0,1) <= 1
 0 <= alpha(1,0,2) <= 1
 0 <= alpha(1,0,3) <= 1
 0 <= alpha(1,1,0) <= 1
 0 <= alpha(1,1,1) <= 1
 0 <= alpha(1,1,2) <= 1
 0 <= alpha(1,1,3) <= 1
 0 <= alpha(1,2,0) <= 1
 0 <= alpha(1,2,1) <= 1
 0 <= alpha(1,2,2) <= 1
 0 <= alpha(1,2,3) <= 1
 0 <= alpha(1,3,0) <= 1
 0 <= alpha(1,3,1) <= 1
 0 <= alpha(1,3,2) <= 1
 0 <= alpha(1,3,3) <= 1
 0 <= alpha(2,0,0) <= 1
 0 <= alpha(2,0,1) <= 1
 0 <= alpha(2,0,2) <= 1
 0 <= alpha(2,0,3) <= 1
 0 <= alpha(2,1,0) <= 1
 0 <= alpha(2,1,1) <= 1
 0 <= alpha(2,1,2) <= 1
 0 <= alpha(2,1,3) <= 1
 0 <= alpha(2,2,0) <= 1
 0 <= alpha(2,2,1) <= 1
 0 <= alpha(2,2,2) <= 1
 0 <= alpha(2,2,3) <= 1
 0 <= alpha(2,3,0) <= 1
 0 <= alpha(2,3,1) <= 1
 0 <= alpha(2,3,2) <= 1
 0 <= alpha(2,3,3) <= 1
 0 <= alpha(3,0,0) <= 1
 0 <= alpha(3,0,1) <= 1
 0 <= alpha(3,0,2) <= 1
 0 <= alpha(3,0,3) <= 1
 0 <= alpha(3,1,0) <= 1
 0 <= alpha(3,1,1) <= 1
 0 <= alpha(3,1,2) <= 1
 0 <= alpha(3,1,3) <= 1
 0 <= alpha(3,2,0) <= 1
 0 <= alpha(3,2,1) <= 1
 0 <= alpha(3,2,2) <= 1
 0 <= alpha(3,2,3) <= 1
 0 <= alpha(3,3,0) <= 1
 0 <= alpha(3,3,1) <= 1
 0 <= alpha(3,3,2) <= 1
 0 <= alpha(3,3,3) <= 1
 0 <= alpha(4,0,0) <= 1
 0 <= alpha(4,0,1) <= 1
 0 <= alpha(4,0,2) <= 1
 0 <= alpha(4,0,3) <= 1
 0 <= alpha(4,1,0) <= 1
 0 <= alpha(4,1,1) <= 1
 0 <= alpha(4,1,2) <= 1
 0 <= alpha(4,1,3) <= 1
 0 <= alpha(4,2,0) <= 1
 0 <= alpha(4,2,1) <= 1
 0 <= alpha(4,2,2) <= 1
 0 <= alpha(4,2,3) <= 1
 0 <= alpha(4,3,0) <= 1
 0 <= alpha(4,3,1) <= 1
 0 <= alpha(4,3,2) <= 1
 0 <= alpha(4,3,3) <= 1
 0 <= r(0) <= 1
 0 <= r(1) <= 1
 0 <= r(2) <= 1
 0 <= r(3) <= 1
 0 <= u(0) <= 4
 0 <= u(1) <= 4
 0 <= v(0,1) <= 1
 0 <= u(2) <= 4
 0 <= v(0,2) <= 1
 0 <= u(3) <= 4
 0 <= v(0,3) <= 1
 0 <= v(1,0) <= 1
 0 <= v(1,2) <= 1
 0 <= v(1,3) <= 1
 0 <= v(2,0) <= 1
 0 <= v(2,1) <= 1
 0 <= v(2,3) <= 1
 0 <= v(3,0) <= 1
 0 <= v(3,1) <= 1
 0 <= v(3,2) <= 1
Binaries
 r(0)  r(1)  r(2)  r(3)  v(0,1)  v(0,2)  v(0,3)  v(1,0)  v(1,2)  v(1,3) 
 v(2,0)  v(2,1)  v(2,3)  v(3,0)  v(3,1)  v(3,2) 
End
