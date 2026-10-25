# Personal Knowledge Vault & Curriculum Index

Welcome to your structured learning curriculum. Every entry links to a permanent, first-principles lesson note with active recall flashcards.

---

## Curriculum Dashboard

| Topic | Category | Status | Core Unconditional Truth | Date Added |
| :--- | :--- | :--- | :--- | :--- |
| [Resolving Executables via PATH](path-searching-executables.md) | Systems Programming (C++) | 🌿 Solid | Inode execute bits (`X_OK`) determine binary execution; direct path query (`dir / target`) avoids folder iteration. | 2026-09-25 |
| [Process Working Directory, `task_struct`, and `pwd` / `cd`](process-working-directory-task-struct.md) | Linux Kernel & Systems (C++) | 🌿 Solid | `task_struct->fs->pwd` stores a live kernel `dentry` pointer copied by value on `fork()`, requiring `cd` to run in-process via syscall. | 2026-10-25 |

---

## Status Legend
- 🌱 **Learning**: Initial exposure and guided discovery in progress.
- 🌿 **Solid**: Successfully implemented and verified with active recall.
- 🌳 **Mastered**: Interleaved and retained across delayed sessions.
