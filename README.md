# Procedural Terrain Generation

A procedural terrain generation project that explores **Diamond-Square** and **Perlin Noise** algorithms to create naturally varying terrain.

The project combines algorithmic terrain generation with image processing and Unity to transform raw procedural data into a smooth, visually designed 3D environment.

## Overview

The project experiments with two different approaches to procedural terrain generation:

* **Diamond-Square Algorithm** — used to generate heightmaps through recursive subdivision and random displacement.
* **Perlin Noise** — selected for producing smoother and more continuous terrain variations.

The generated terrain data is converted into a **grayscale heightmap**, normalized to a `0–1` range, and then rescaled and modified using **GIMP** before being imported into **Unity Engine**.

Unity uses the processed heightmap to generate the final 3D terrain.

### Pipeline

```text
Diamond-Square / Perlin Noise
            ↓
      Heightmap Generation
            ↓
       Grayscale Image
            ↓
       Normalize to 0–1
            ↓
        Rescale / Edit
            ↓
            GIMP
            ↓
     Processed Heightmap
            ↓
          Unity
            ↓
     3D Procedural Terrain
```

## Algorithms

### Diamond-Square

The Diamond-Square algorithm generates terrain by recursively subdividing a grid and introducing controlled random displacement.

It consists primarily of two operations:

* **Diamond Step** — calculates the center of each square from its surrounding corner values.
* **Square Step** — calculates the remaining points using neighboring values.

Repeated subdivision produces a heightmap with increasingly detailed terrain features.

This approach is useful for creating terrain with fractal-like variation, although excessive randomness can result in relatively rough or harsh landscapes.

### Perlin Noise

Perlin Noise generates smooth, continuous pseudo-random values.

Unlike purely random noise, nearby points have related values, which produces gradual transitions between heights.

This made Perlin Noise more suitable for the final terrain because it produces:

* Smoother elevation changes
* More natural-looking landscapes
* Continuous hills and valleys
* Reduced sharp variations

For the final terrain generation, **Perlin Noise was therefore chosen as the primary approach**.

## Heightmap Processing

The generated terrain data is represented as a grayscale image.

Each pixel represents an elevation:

```text
Black  → Lower elevation
White  → Higher elevation
```

The generated values are normalized to the range:

```text
0 → 1
```

This normalized data is then rescaled and further adjusted to obtain a more suitable height distribution.

The resulting heightmap is processed in **GIMP**, where additional visual adjustments can be made before the map is imported into Unity.

## Unity Terrain Generation

The processed grayscale heightmap is imported into **Unity Engine**.

Unity interprets the brightness of each pixel as terrain elevation and converts the 2D heightmap into a 3D terrain surface.

This produces a smooth, randomly generated landscape containing features such as:

* Hills
* Valleys
* Elevated regions
* Low-lying regions
* Natural elevation transitions

The combination of procedural generation and heightmap processing allows the terrain to be generated while still providing control over its final appearance.

## Technologies Used

* **Python** — procedural terrain generation
* **Diamond-Square Algorithm** — fractal terrain generation
* **Perlin Noise** — smooth terrain generation
* **GIMP** — heightmap processing and adjustments
* **Unity Engine** — 3D terrain generation and visualization

## Project Workflow

### 1. Generate Terrain Data

Procedural algorithms generate elevation values across a 2D grid.

### 2. Generate a Heightmap

The elevation values are converted into a grayscale image.

### 3. Normalize the Data

Terrain values are scaled into the `0–1` range to represent relative elevation.

### 4. Rescale and Process

The heightmap is rescaled and adjusted to produce a more suitable terrain distribution.

### 5. Edit in GIMP

The heightmap can be further modified to control the final terrain shape and appearance.

### 6. Import into Unity

The processed heightmap is imported into Unity as a terrain heightmap.

### 7. Generate the 3D Environment

Unity converts the heightmap into a 3D terrain, resulting in a smooth procedurally generated landscape.

## Results

The final result is a **smooth, randomly generated 3D terrain** produced from procedural height data.

The project demonstrates how procedural algorithms can be combined with image processing and a game engine to transform numerical terrain data into a visually interpretable 3D environment.

## Future Improvements

Possible extensions include:

* Generating terrain entirely within Unity
* Adding procedural terrain textures
* Automatically generating biomes based on elevation
* Adding rivers and erosion simulation
* Implementing multiple levels of terrain detail
* Adding procedural vegetation
* Generating larger terrain worlds
* Experimenting with different noise functions and parameters

## Learning Outcomes

Through this project, we explored:

* Procedural content generation
* Terrain heightmaps
* Diamond-Square terrain generation
* Perlin Noise
* Data normalization and rescaling
* Grayscale image representation
* Image-based terrain generation
* Unity Terrain
* Combining algorithmic generation with artistic post-processing

---

## Project Structure

```text
Procedural-Terrain-Generation/
│
├── src/
│   └── terrain generation code
│
├── heightmaps/
│   └── generated terrain maps
│
├── unity/
│   └── Unity terrain project
│
├── README.md
└── ...
```

## Authors

Developed as a procedural terrain generation project exploring algorithmic approaches to creating natural-looking landscapes.
