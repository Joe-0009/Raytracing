flowchart TD
    A[Pixel Coordinates x,y] --> B[generate_camera_ray]
    B --> C{Calculate Camera Basis}
    C --> D[Calculate Right Vector]
    C --> E[Calculate Up Vector]
    D --> F[Transform to World Space]
    E --> F
    F --> G[Create Ray Object]
    
    G --> H[trace_ray]
    H --> I[trace_objects]
    I --> J{For Each Object}
    
    J --> K{Object Type?}
    K -->|Cylinder| L[trace_cylinder]
    K -->|Cone| M[trace_cone]
    K -->|Sphere| N[trace_sphere]
    K -->|Plane| O[trace_plane]
    
    L --> P[intersect_cylinder]
    M --> Q[intersect_cone]
    
    P --> R{Check Caps First}
    R -->|Hit Cap| S[check_cap_hit]
    R -->|No Cap Hit| T[Check Cylinder Surface]
    T --> U[cylinder_quadratic_coeffs]
    U --> V[solve_quadratic]
    V --> W{Valid Intersection?}
    W -->|Yes| X[Calculate Surface Normal]
    W -->|No| Y[Return No Hit]
    
    Q --> Z{Check Cone Surface}
    Z --> AA[cone_quadratic_coeffs]
    AA --> BB[solve_quadratic]
    BB --> CC{Valid m value?}
    CC -->|Yes| DD[cone_surface_normal]
    CC -->|No| EE[Check Cone Cap]
    EE --> FF[intersect_cone_cap]
    
    S --> GG[Set Hit Data]
    X --> GG
    DD --> GG
    FF --> GG
    
    GG --> HH{Closest Hit?}
    HH -->|Yes| II[Update closest_hit]
    HH -->|No| JJ[Continue Loop]
    
    II --> KK[calculate_lighting]
    KK --> LL[Calculate Ambient]
    KK --> MM[calculate_diffuse]
    MM --> NN[is_in_shadow]
    NN --> OO{In Shadow?}
    OO -->|No| PP[Apply Diffuse Lighting]
    OO -->|Yes| QQ[Skip Diffuse]
    
    PP --> RR[Combine Ambient + Diffuse]
    QQ --> RR
    RR --> SS[clamp_color]
    SS --> TT{Selected Object?}
    TT -->|Yes| UU[apply_selection_highlight]
    TT -->|No| VV[color_to_int]
    UU --> VV
    
    VV --> WW[Return Final Color]
    
    Y --> XX[Return Sky Color]
    JJ --> XX
    
    style A fill:#e1f5fe
    style WW fill:#c8e6c9
    style XX fill:#fff3e0