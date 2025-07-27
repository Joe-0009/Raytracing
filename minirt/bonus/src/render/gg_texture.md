flowchart TD

A[intersect_sphere] -->|1. Calculate hit| B[calculate_sphere_hit]
B -- success --> C[apply_sphere_bump]
C --> D[determine_sphere_color]
D --> E{Texture active?}
E -- Yes --> F[sample_texture]
E -- No, checkerboard --> G[checkerboard color]
E -- No, solid --> H[sphere->color]

C -->|Bump active?| I[apply_bump_mapping]
I --> J[sample_bump_map (center, right, up)]
J --> K[compute du, dv]
K --> L[perturb normal & normalize]

A -.->|If ray hits backface| M[flip normal]

B -.->|fail| N[return 0 (no hit)]