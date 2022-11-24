# lstools

### compile

- Target machines and build machines are one
```
source env.sh host
make
```
- Target machine and build is not one and we need to cross compile
```
source env.sh loongarch64
make
```

### Add new module
1. env.sh add *_SUPPORT
2.Add dir
3. min.c add call
4. app.mak add compile
