# simdjson 的 php 扩展
使用 PHP-CPP 编写，编译的时候要先添加 PHP-CPP 的库，查看 http://www.php-cpp.com/documentation/install

# 编译安装

```sh
make && make install
```
在php.ini里添加 extension=simdjson.so

# 使用方法

```php
simdjson_isvalid(json) // 判断是不是json
simdjson_decode(json,[assoc]) // 解析成对象或数组
```

# 不推荐使用
性能不如这个[simdjson_php](https://github.com/crazyxman/simdjson_php)，甚至比 json_decode 都差。不过我对比了多个扩展后得出这个[php-rapidjson](https://github.com/rustjason/php-rapidjson)性能是最好的。