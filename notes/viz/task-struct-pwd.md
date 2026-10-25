# Kernel Layout: `task_struct` to `pwd`

When the Linux kernel tracks where your process currently is, it uses a three-level structure defined across `<linux/sched.h>`, `<linux/fs_struct.h>`, and `<linux/path.h>`.

```mermaid
graph LR
    subgraph Process Descriptor ["include/linux/sched.h"]
        TS["<b>struct task_struct</b><br/>pid_t pid<br/>struct fs_struct *fs"]
    end

    subgraph Filesystem State ["include/linux/fs_struct.h"]
        FS["<b>struct fs_struct</b><br/>int users<br/>struct path root<br/>struct path pwd"]
    end

    subgraph VFS Path ["include/linux/path.h"]
        P["<b>struct path (pwd)</b><br/>struct vfsmount *mnt<br/>struct dentry *dentry"]
    end

    subgraph Dentry Cache ["VFS Directory Tree (RAM)"]
        D_CUR["<b>dentry: 'codecrafters-shell-cpp'</b><br/>d_inode ──► Inode #40912"]
        D_PAR["<b>dentry: 'repos'</b>"]
        D_ROOT["<b>dentry: '/'</b>"]
    end

    TS -->|fs| FS
    FS -->|pwd| P
    P -->|dentry| D_CUR
    D_CUR -->|d_parent| D_PAR
    D_PAR -->|d_parent| D_ROOT
```

## Exact Kernel C Definitions (Simplified)

```c
// 1. include/linux/sched.h
struct task_struct {
    // ... scheduler state, PID, credentials ...
    struct fs_struct *fs;  // Filesystem information pointer
    struct files_struct *files; // Open file descriptors
};

// 2. include/linux/fs_struct.h
struct fs_struct {
    int users;             // Reference count (shared by threads, copied on fork)
    spinlock_t lock;
    seqcount_spinlock_t seq;
    int umask;
    int in_exec;
    struct path root;      // Process root directory (usually /)
    struct path pwd;       // Present Working Directory!
};

// 3. include/linux/path.h
struct path {
    struct vfsmount *mnt;  // Which mounted filesystem (e.g., ext4 on /dev/nvme0n1p2)
    struct dentry *dentry; // Pointer to the live directory node in the VFS tree
};
```

---

## What Happens During `fork()` (`copy_fs` in `kernel/fork.c`)

When a shell calls `fork()`, the kernel allocates a **new** `task_struct` and a **new** `fs_struct` for the child, copying the `pwd.dentry` pointer value from the parent:

```mermaid
graph TD
    subgraph Parent Shell Process
        P_TS["Parent <b>task_struct</b>"] -->|fs| P_FS["Parent <b>fs_struct</b> (@ 0x1000)<br/>pwd.dentry"]
    end

    subgraph Child Process ["Child Process (after fork)"]
        C_TS["Child <b>task_struct</b>"] -->|fs| C_FS["Child <b>fs_struct</b> (@ 0x2000)<br/>pwd.dentry (copied!)"]
    end

    subgraph Kernel Dentry Cache
        D_HOME["<b>dentry: '/home/adam'</b>"]
        D_TMP["<b>dentry: '/tmp'</b>"]
    end

    P_FS -->|points to| D_HOME
    C_FS -.->|"1. Right after fork(): points to"| D_HOME
    C_FS ==>|"2. After child calls chdir('/tmp'): points to"| D_TMP
```

- **Why `/bin/pwd` works in a child**: Right after `fork()`, the child's `fs_struct->pwd` points to the exact same `dentry` (`/home/adam`) as the parent. Reading and printing it gives the parent's current directory.
- **Why `/bin/cd` cannot work in a child**: Calling `chdir("/tmp")` inside the child only repoints the **child's** `fs_struct->pwd` (`@ 0x2000`) to `/tmp`. Then the child exits and `@ 0x2000` is freed—leaving the parent shell's `fs_struct->pwd` (`@ 0x1000`) untouched at `/home/adam`!

