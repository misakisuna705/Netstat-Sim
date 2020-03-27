# Ubuntu

## 編譯

### make

```zsh
make
```

### cmake

```zsh
cmake . -DCMAKE_BUILD_TYPE=Release -Bbuild/release && make -C ./build/release
```

## 執行

```zsh
bin/main
```

## 清理

```zsh
make clean
```
