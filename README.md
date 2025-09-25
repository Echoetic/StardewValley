# 🎣 Stardew Valley Minigames Recreation in C++ 🎯

A C++ project dedicated to recreating the engaging minigames found within the popular game, **Stardew Valley**. This repository aims to provide faithful, standalone, open-source implementations of games like the Fishing minigame and the Dart minigame (often found in the Stardew Valley arcade).

This project uses modern C++ and the **EasyX Graphics Library** for rendering and user interaction, making it ideal for Windows environments and C++ learning.

---

## ✨ Features

* **🎣 Fishing Minigame (The Green Bar):** A near-perfect recreation of the challenging fishing mechanic.
    * Responsive control of the **Fishing Bar** using mouse input (`WM_LBUTTONDOWN`/`WM_LBUTTONUP`).
    * Simulated physics for the bar, including speed accumulation and bounce-off-edge dampening.
    * Randomized movement and speed of the **Fish** target.
    * A **Progress Bar** that fills when the bar is successfully kept on the fish and drains otherwise.
    * Visual feedback through color changes on the fishing bar.

* **🎯 Dart Minigame (Arcade Style):** A recreation of a classic darts game, likely inspired by the one in the Stardew Valley arcade.
    * A dynamic **SolidCircle** target that randomly moves and changes size, controlled by arrow keys/WASD.
    * Implements the classic **301 Darts** scoring system, including single, double, and triple rings, as well as the Bull's-Eye (50 points) and Outer Ring (25 points).
    * Throwing a dart (`'J'` key) calculates the score based on the target's current position and radius.
    * Multi-round structure with game-over logic for failing to hit exactly 301.

---

## 🛠️ Technologies Used

* **Language:** C++
* **Graphics Library:** [**EasyX**](https://docs.easyx.cn/) (A simple, powerful graphics library for Windows C++ programming)
* **Development Environment:** Primarily built for a Windows environment.

---

## 🚀 Getting Started

### Prerequisites

To compile and run this project, you will need:

1.  A C++ compiler (e.g., MinGW, MSVC).
2.  The **EasyX Library** installed and configured in your IDE (Visual Studio is commonly used with EasyX).

### Compilation and Run

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Echoetic/StardewValley.git
    cd src
    ```
2.  **Add Assets:** Ensure the required image files (like `fish.png`, `fishbk.png`, `back.png`, `dart.jpg`, `dartbk.jpg`, etc., as referenced in the code) are placed in the same directory as the compiled executable.
3.  **Compile:** Compile the C++ source files using your EasyX-configured environment.
4.  **Run:** Execute the compiled program.

---

## 🎮 How to Play

### Fishing Minigame

* **Goal:** Keep the green **Fishing Bar** (left side) aligned with the moving **Fish** target.
* **Controls:**
    * **Hold Left Mouse Button Down:** The fishing bar accelerates upwards.
    * **Release Left Mouse Button:** The fishing bar accelerates downwards.
* **Winning:** Fill the **Progress Bar** (right side) completely.
* **Losing:** Let the Progress Bar drain completely.

### Dart Minigame (301)

* **Goal:** Score exactly **301 points** in three rounds or fewer.
* **Controls:**
    * **WASD / Arrow Keys:** Move the current target circle's position on the dartboard.
    * **'J' Key:** Throw the dart (registers a hit at the target circle's current position).
* **Mechanics:**
    * The target circle automatically moves and changes size (`RMIN` to `RMAX`).
    * Scoring follows standard 301 rules: Outer rings grant double/triple the segment score. Bullseye is 50, Outer Bull is 25.
    * If a throw causes the score to exceed 301, the throw is a "LOSS," and the score remains unchanged.

---

## 📝 Code Highlights

* **`FishingBar` Class:** Contains movement logic, speed calculation (`update_speed`), and boundary checks (bouncing off 24px margins).
* **`Fish` Class:** Uses a simple randomized direction (`update_direction_randomly`) and speed logic for movement.
* **`SolidCircle` Class (Darts):** Implements automatic (randomized `dx`/`dy`) and manual (keyboard `VCTRL`) movement, along with radius oscillation (`dr`).
* **`MainBody::compute_dscore` (Darts):** Calculates the base segment score (1 to 20) using trigonometry (`atan` logic on `tantheta` is effectively implemented with a large chain of `tan` comparisons) to determine which dartboard wedge the center of the target falls into.
* **Game Loop:** Both minigames use a core game loop structure with `BeginBatchDraw()` and `FlushBatchDraw()` for smooth animation at a fixed framerate (e.g., $40\text{ FPS}$ for fishing, $30\text{ FPS}$ for darts).
