Wrzucił Pan do repozytorium kartotekę build. Proszę ją usunąć. 

# House 2/2

20.10.2025 11:56:36 OK


# Colors 5/5 

20.10.2025 11:57:11 OK

# Indices 5/5

12.11.2025 09:54:03 OK

# Uniforms 8/8

12.11.2025 09:55:37 OK

# PVM 5/5

12.11.2025 09:56:41 OK

# Pyramid 3/3

12.11.2025 09:58:39 OK

# Resize 8/8

12.11.2025 10:00:00 OK

# Zoom 10/10

17.11.2025 11:08:44 OK

# Camera movement 15/15

24.11.2025 10:21:17 OK
Ale LBM_pressed powinien być ustawiony na false w konstruktorze. 

# Mesh  12/12

1.12.2025 10:15:51 OK

# Kd material  20/20

15.12.2025 10:37:55 OK

8.12.2025 10:23:33 OK
Brak możliwośći ustawienia use_vertex colors w konstruktorze. Dlaczegi jest to mutable?

# Textures 15/15

12.01.2026 10:36:38 OK

# OBJ reader 

19.01.2026 12:57:34 OK

# Diffuse 0/25

14.02.2026 11:38:07 
Offsety przy swiatlach są dalej zle. Upakowanie w bufurze powinno wygladac tak:

|A_r|A_g|A_b|n_lights|l[0]_x|l[0]_y|l[0]_z|l[0]_radius|l[0]_r|l[0]_g|l[0]_b|l[0]_intensity|....


4.02.2026 11:40:39
Złe offsety przy przesyłaniu swiatla. n_light ma offset 3*sizeof(float).
