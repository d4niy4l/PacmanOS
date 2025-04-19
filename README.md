# 🟡 Pacman Reimagined with Threads & SFML

A fun and visually rich recreation of the classic **Pacman** game, built from the ground up in C++ using **pthreads**, **mutexes**, **semaphores**, and advanced **concurrency patterns**. Featuring ghost AI that mimics the original arcade logic and a smooth graphical interface using **SFML**.

---

## 🎮 Features

- 🧵 **Multithreaded Architecture**: Built using `pthreads` to ensure smooth and responsive gameplay.
- 🔐 **Synchronization Mechanisms**: Utilizes `mutexes` and `semaphores` to safely handle concurrent access.
- 📦 **Concurrency Design Patterns**:
  - **Producer-Consumer**: For managing game events and resource sharing.
  - **Cigarette Smokers Problem**: Applied creatively in game logic synchronization.
  - **Dining Philosophers/Table Pattern**: Used to manage shared resources elegantly.
  - And other classic concurrency problems reimagined in a gaming context.
- 👻 **Ghost AI**:
  - Each ghost follows unique movement logic inspired by the original game: Blinky, Pinky, Inky, and Clyde each have distinct personalities.
- 🎨 **Beautiful Interface**:
  - Developed using **SFML** for smooth graphics, animations, and an engaging experience.
- 😄 **Fun to Play**:
  - Challenging, nostalgic, and enhanced with modern programming practices.

---

## 🖥️ Technologies Used

- **C++**
- **POSIX Threads (pthreads)**
- **SFML (Simple and Fast Multimedia Library)**
- **Synchronization Primitives**: `mutex`, `semaphore`, condition variables

---

## 🚀 Getting Started (Linux)

> Make sure you have `SFML` installed on your system.

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/pacman-threads-sfml.git
cd pacman-threads-sfml
```

### 2. Run the Game

```bash
./run.sh
```

The script will handle building and launching the game automatically.

---

## 🧠 Concurrency Patterns Applied

| Pattern                | Applied In                        |
|------------------------|-----------------------------------|
| Producer-Consumer      | Game events, power-up handling    |
| Cigarette Smokers      | Coordinating ghost behavior       |
| Dining Philosophers    | Resource sharing (e.g. map tiles) |
| Table Problem          | Game state synchronization        |

---

## 🖼️ Screenshots

![Pacman](https://github.com/user-attachments/assets/64e529d9-f843-4ecd-a519-4153399b60ed)

---

## 📚 Credits

- Inspired by the original Pacman by Namco.
- GUI powered by [SFML](https://www.sfml-dev.org/).
- Developed with love for OS and Systems Programming ❤️

---

## 🔮 Future Enhancements

- Multiplayer mode
- Difficulty levels
- Custom maps and levels
- Sound effects and background music

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

---
