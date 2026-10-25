# C++ Move Semantics: Pointer Theft vs. Deep Copy

At the hardware level, both `std::string` and `std::vector` are small stack structs containing a **pointer (`data`)** to a heap buffer, plus `size` and `capacity`.

---

## 1. Deep Copy (`std::string b = a;`)

Allocates a **new** heap buffer and copies every byte ($O(N)$):

```mermaid
graph LR
    subgraph Stack
        A["<b>std::string a</b><br/>data: 0x5000<br/>size: 10"]
        B["<b>std::string b</b><br/>data: 0x9000<br/>size: 10"]
    end

    subgraph Heap
        H1["<b>@ 0x5000</b><br/>'helloworld'"]
        H2["<b>@ 0x9000 (Newly Allocated!)</b><br/>'helloworld'"]
    end

    A -->|points to| H1
    B -->|points to| H2
```

---

## 2. Move (`std::string b = std::move(a);`)

Allocates **nothing** ($O(1)$). `b` steals `a`'s heap pointer, and resets `a.data = nullptr` so `a`'s destructor won't free the buffer:

```mermaid
graph LR
    subgraph Stack
        A_AFTER["<b>std::string a (emptied)</b><br/>data: nullptr<br/>size: 0"]
        B_AFTER["<b>std::string b (new owner)</b><br/>data: 0x5000<br/>size: 10"]
    end

    subgraph Heap
        H_SHARED["<b>@ 0x5000 (Same Buffer!)</b><br/>'helloworld'"]
    end

    A_AFTER -.->|nullptr| H_SHARED
    B_AFTER ==>|stole pointer!| H_SHARED
```

---

## In 5 Lines of C++ (How Move Constructor Actually Works)

```cpp
struct SimpleString {
  char *data;
  size_t size;

  // Move operation ("steal the pointer"):
  void MoveFrom(SimpleString &source) {
    this->data = source.data;   // 1. Point to source's existing heap array
    this->size = source.size;
    source.data = nullptr;      // 2. Null out source so its destructor does nothing!
    source.size = 0;
  }
};
```
