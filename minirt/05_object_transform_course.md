# Course 5: Object Transformations (Translation & Rotation)

## 1. Introduction

Transformations let you place and orient objects and cameras in 3D space. For miniRT, support **translation** for all objects, and **rotation** for everything except spheres and lights.

---

## 2. Translation

- Move an object by adding a vector to its position.
- For camera and objects: modify their position vectors before intersection tests.

---

## 3. Rotation

- Only applies to objects with orientation (plane normals, cylinder axes, camera direction).
- Use rotation matrices or quaternion math.
- For each object, store its orientation vector.
- To rotate, multiply orientation by a rotation matrix (around x, y, or z axes).

---

## 4. Implementation Steps

- Parse translation and rotation from scene file or apply through code.
- Update object/camera properties accordingly before raytracing.
- For rotated planes/cylinders/cameras:
    - Rotate normal/axis/direction.
    - Ensure the vectors remain normalized.

---

## 5. Example: Rotating a Vector

```c
void rotate_vector(t_vec3 *v, float angle, t_vec3 axis) {
    // apply rotation matrix for axis and angle
}
```

---

## 6. Testing

- Place objects in different locations and orientations.
- Rotate planes/cylinders/cameras and verify visually.
- Ensure all intersection logic uses the updated positions/orientations.

---

## 7. Conclusion

Transformations allow for dynamic, interesting scenes and are foundational for animation, modeling, and camera control.