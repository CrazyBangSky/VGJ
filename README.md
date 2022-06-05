# UE4建造系统

##### 说明
* UE4.26
* 键位说明

| 按键 | 功能 |
|--| --|
| B | 启用建造系统 |
| N | 关闭建造系统 |
| Q、E | 切换要建造的东西 |
| R | 旋转要建造的东西 |
| 鼠标左键 | 建造东西 |
| C | 拾取东西 |

* 演示视频URl：https://www.bilibili.com/video/BV1av411p7JL
* 主要用c++实现。代码高内聚低耦合，容易复用到新工程
* 分为 拾取系统 、建造系统。两个互不依赖，用在一起可以实现演示视频里的那种效果：拾取东西，建造它，又把它捡回来。二者在character的代码里产生了关联。
	* 要用这两个系统的话，pawn需要实现对应的接口。对应拾取系统的是IPicker,对应建造系统的是IMyBuildSystemBuilder
	* 每个系统的核心实现主要都写在一个ActorComponent里，功能解耦。
* 如何新增一个 可建造、可拾取的东西
	* 网上搜索下载得到static mesh
		* 在ue4中将static mesh的轴调整到static mesh的 底部中间。不然建造预览效果会和实际建造效果不一样。
		* static mesh的尺寸最好调整到理想的，后期也可以通过调整结构体中的GhostScale来调整。
	* 照着已有的Pickable Actor弄一个   

* 核心实现参考：https://www.youtube.com/watch?v=nSD4dbu5Ek4&list=PLkNi6pHiTfNk1mGJoWmiJUbEGKCv6usgI