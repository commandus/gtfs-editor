Simplified GTFS editor
======================

### Edit

```
(F8 route_short_name ^) 
(F9 01.01.2017 - 31.12.2017 ^)
(F1..F7 ПнВтСрЧтПтСбВс)
+-------------+   +-------------+  +---------------+
| Отправление |   | - Выходные  |  | + Рабочие дни |
+-------------+   +-------------+  +---------------+
+09:00
 11:30
 
 (F10- Save)
```


### Libraries
Add the following line to /etc/apt/sources.list

deb http://ppa.launchpad.net/keithw/glfw3/ubuntu trusty main

sudo apt-get update
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
