# Ray/Cylinder and Ray/Cone Intersection – Two Practical Mini-Courses

---

# Course 1: Ray/Cylinder Intersection

## 1. Introduction

- **Cylinders** are another common primitive in ray tracing.
- Ray-cylinder intersection is required for rendering, collision detection, and bounding volume tests.
- We focus on **infinite cylinders** (aligned with an axis) and touch on finite (capped) cylinders.

---

## 2. Mathematical Definition

- A cylinder of radius `r` centered at point `C` and axis direction `V` (normalized) has the implicit equation:
  ```
  |(P - C) - ((P - C)·V)V|^2 = r^2
  ```
  - `P` is a point on the cylinder.
  - This means: the squared distance from `P` to the axis line is `r^2`.

---

## 3. Ray Equation

- A ray is defined as:
  ```
  R(t) = O + tD
  ```
  - `O` is the origin, `D` is the (normalized) direction, `t >= 0`.

---

## 4. Intersection Derivation

1. Substitute `R(t)` into the cylinder equation:
   ```
   |(O + tD - C) - ((O + tD - C)·V)V|^2 = r^2
   ```
2. Let `X = O - C`.
3. Project X and D onto the axis:
   - `a = D - (D·V)V`
   - `b = X - (X·V)V`
4. Plug back:
   ```
   |b + t*a|^2 = r^2
   => (b + t*a)·(b + t*a) = r^2
   => (a·a)t^2 + 2(a·b)t + (b·b - r^2) = 0
   ```
   This is a **quadratic equation** in t.

---

## 5. Implementation Steps

**Step 1:** Compute `V` (axis, normalized), `C` (point on axis), `r` (radius).

**Step 2:** For a ray `O + tD`:
- Compute `X = O - C`
- Compute projections:
  - `a = D - (D·V)V`
  - `b = X - (X·V)V`

**Step 3:** Compute coefficients:
- `A = a·a`
- `B = 2(a·b)`
- `C = b·b - r^2`

**Step 4:** Solve quadratic `A*t^2 + B*t + C = 0`
- Discriminant `D = B^2 - 4AC`
- If `D < 0`: no intersection
- Else, two solutions for t; pick smallest positive t

**Step 5 (for finite cylinders):**
- Compute intersection point `P = O + tD`
- Check if `|((P - C)·V)|` is within half the cylinder height

---

## 6. Example C++-like Code

```cpp
bool intersectCylinder(const Vec3f &O, const Vec3f &D, // Ray origin, dir (normalized)
                       const Vec3f &C, const Vec3f &V, // Cylinder point, axis (normalized)
                       float r, float height, float &t) {
    Vec3f X = O - C;
    Vec3f a = D - V * dot(D, V);
    Vec3f b = X - V * dot(X, V);
    float A = dot(a, a);
    float B = 2 * dot(a, b);
    float C = dot(b, b) - r*r;
    float disc = B*B - 4*A*C;
    if (disc < 0) return false;
    float sqrt_disc = sqrt(disc);
    float t0 = (-B - sqrt_disc)/(2*A), t1 = (-B + sqrt_disc)/(2*A);
    if (t0 > t1) std::swap(t0, t1);
    float y0 = dot(V, (O + t0*D) - C);
    float y1 = dot(V, (O + t1*D) - C);
    // For infinite cylinder, accept any t>0; for finite, check |y| < height/2
    if (t0 >= 0 && fabs(y0) <= height/2) { t = t0; return true; }
    if (t1 >= 0 && fabs(y1) <= height/2) { t = t1; return true; }
    return false;
}
```

---

## 7. Next Steps

- For **capped cylinders**, also check intersection with the two end planes (disks) as in ray-disk intersection.
- Use the surface normal for shading:
  ```
  N = ((P - C) - ((P - C)·V)V) / r
  ```

---

# Course 2: Ray/Cone Intersection

## 1. Introduction

- **Cones** are another analytic primitive.
- Useful for spotlights, geometry, and bounding volumes.
- Here, we focus on infinite cones aligned with an axis, then discuss limiting to a finite height.

---

## 2. Mathematical Definition

- A cone with apex point `A`, axis direction `V` (normalized), and half-angle θ (opening angle).
- Any point `P` on the cone satisfies:
  ```
  ((P - A)·V)^2 = |P - A|^2 * cos^2(θ)
  ```
  - The angle between (P - A) and V is θ.

---

## 3. Ray Equation

- Ray: `R(t) = O + tD`
- Substituting into cone equation:
  - Let `X = O - A`
  - `m = cos(θ)`

---

## 4. Intersection Derivation

1. Substitute into the cone's equation:
   ```
   ((D·V)t + (X·V))^2 = (|D*t + X|^2) * m^2
   ```
2. Expand right side:
   ```
   (D·V)^2 t^2 + 2(D·V)(X·V)t + (X·V)^2 = (D·D t^2 + 2 D·X t + X·X) * m^2
   ```
3. Rearranged as quadratic:
   ```
   At^2 + Bt + C = 0
   ```
   Where:
   - `A = (D·V)^2 - m^2 * (D·D)`
   - `B = 2(D·V)(X·V) - 2m^2(D·X)`
   - `C = (X·V)^2 - m^2(X·X)`

---

## 5. Implementation Steps

**Step 1:** Compute `V` (axis, normalized), `A` (apex), θ (cone angle in radians).

**Step 2:** For ray `O + tD`:
- `X = O - A`
- `m = cos(θ)`

**Step 3:** Compute coefficients:
- `dv = D·V`
- `xv = X·V`
- `dd = D·D`
- `dx = D·X`
- `xx = X·X`
- `A = dv*dv - m*m*dd`
- `B = 2*dv*xv - 2*m*m*dx`
- `C = xv*xv - m*m*xx`

**Step 4:** Solve quadratic `A*t^2 + B*t + C = 0`
- Discriminant `D = B^2 - 4AC`
- If `D < 0`: no intersection
- Else, two solutions for t; pick smallest positive t

**Step 5 (for finite cones):**
- Check that the intersection point lies within the desired height along the axis:
  - Compute y = (P - A)·V
  - Accept if 0 < y < height

---

## 6. Example C++-like Code

```cpp
bool intersectCone(const Vec3f &O, const Vec3f &D, // Ray origin, dir (normalized)
                   const Vec3f &A, const Vec3f &V, // Apex, axis (normalized)
                   float theta, float height, float &t) {
    Vec3f X = O - A;
    float m = cos(theta);
    float dv = dot(D, V), xv = dot(X, V);
    float dd = dot(D, D), dx = dot(D, X), xx = dot(X, X);
    float A_coef = dv*dv - m*m*dd;
    float B_coef = 2*dv*xv - 2*m*m*dx;
    float C_coef = xv*xv - m*m*xx;
    float disc = B_coef*B_coef - 4*A_coef*C_coef;
    if (disc < 0) return false;
    float sqrt_disc = sqrt(disc);
    float t0 = (-B_coef - sqrt_disc)/(2*A_coef), t1 = (-B_coef + sqrt_disc)/(2*A_coef);
    if (t0 > t1) std::swap(t0, t1);
    float y0 = dot(V, (O + t0*D) - A);
    float y1 = dot(V, (O + t1*D) - A);
    // For infinite cone, accept any t>0; for finite, check 0 < y < height
    if (t0 >= 0 && y0 > 0 && y0 < height) { t = t0; return true; }
    if (t1 >= 0 && y1 > 0 && y1 < height) { t = t1; return true; }
    return false;
}
```

---

## 7. Next Steps

- For **capped cones (truncated cones)**, also check intersection with the base disk at the desired height.
- For **double cones**, allow negative y as well.
- Surface normal at intersection:
  ```
  N = normalize((P - A) - V * |P - A| * cos(θ))
  ```

---

# Summary

- Both **cylinder** and **cone** intersections reduce to quadratic equations.
- For finite objects, always check if the intersection point lies within the object's bounds.
- Use surface normals for shading and reflection calculations.
- For capped versions, combine with plane/disk intersection tests.

---