# 如何添加一个三方库到OpenHarmony的thirdparty子系统中
## 简介
OpenHarmony整体遵从分层设计，从下向上依次为：内核层、系统服务层、框架层和应用层。系统功能按照“系统 > 子系统 > 组件”逐级展开，在多设备部署场景下，支持根据实际需求裁剪某些非必要的组件。
### 什么是子系统
子系统是一个逻辑概念，它具体由对应的组件构成。
### 什么是组件
组件是对子系统的进一步拆分，可复用的软件单元，它包含源码、配置文件、资源文件和编译脚本；能独立构建，以二进制方式集成，具备独立验证能力的二进制单元。
## 三方库添加到OpenHarmony的thirdparty子系统的步骤
在OpenHamony系统中适配完一个三方库后，为了方便管理与引用，我们需要将三方库添加到thirdparty子系统中，通过thirdparty子系统将我们的三方库添加到系统的编译构建体系中。 
### 定义组件
- 在适配完的三方库目录下新建一个bundle.json文件，内容如下：
  ```json
   {
   	"component": {                                   # 组件属性
         "name": "<component_name>",                   # 组件名称
         "subsystem": "",                              # 组件所属子系统
         "syscap": [],                                 # 组件为应用提供的系统能力
         "features": [],                               # 组件对外的可配置特性列表，一般与build中的sub_component对应，可供产品配置
         "adapted_system_type": [],                    # 轻量(mini)小型(small)和标准(standard)，可以是多个
         "rom": "xxxKB"                                # ROM基线，没有基线写当前值
         "ram": "xxxKB",                               # RAM基线，没有基线写当前值
         "deps": {
             "components": [],                         # 组件依赖的其他组件
             "third_party": []                         # 组件依赖的三方开源软件
         },
         "build": {                                    # 编译相关配置
             "sub_component": [],   					 # 组件编译入口，新增模块在此处配置
             "inner_kits": [],                         # 组件间接口
             "test": []                                # 组件测试用例编译入口
         }
     }
   }
  ```
- 配置子系统为thirdparty子系统
  subsystem为组件所属子系统，将其值配置为thirdparty：
  ```
  "subsystem": "thirdparty", 
  ```
- 配置编译目标
  在bundle.json的build模块中，sub_component 为需要编译的目标，此目标定义在适配完的三方库BUILD.gn中。sub_component可以配置多个目标。
  如果组件有测试用例的话，还可以配置test选项。
  ```json
  	"build": {                                    		
  		"sub_component": [ 
              "//third_party/libraryname:library_name1",
              "//third_party/libraryname:library_name2"
          ]
  		"inner_kits": [],                         		
  		"test": [ "//third_party/libraryname:librarytest" ]
  	}
  ```
- 将目标加入组件中
  在三方库的BUILD.gn中，将对应的目标添加到组件中:
  ```
  import("//build/ohos.gni")
  ohos_shared_library( "library_name1" ) {
  	...
      part_name = “component_name”
  }
  ohos_shared_library( "library_name2" ) {
  	...
      part_name = “component_name”
  }
  ohos_executable( “exe_name” ){
      ......
      part_name = “component_name”
  }
  ```
### 引用组件
定义完组件后，需要加入产品定义中，产品定义文件是//vendor/{product_company}/{product-name}/config.json，在该文件中找到thirdparty，并将新增组件<component_name>添加到thirdparty子系统中。
```
...
{
      "subsystem": "thirdparty",
      "components": [					
        ...									# 原有其他组件
        {									# 新增component_name组件
          "component": "component_name",
          "features": []
        }
      ]
},
...
```
## 参考资料
- [编译构建指导](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/subsystems/subsys-build-all.md)。
- [OpenHarmony 编译构建](https://gitee.com/openharmony/build/blob/master/README_zh.md)。
- [OpenHarmony三方库组织地址](https://gitee.com/openharmony-tpc)。 
- [知识体系](https://gitee.com/openharmony-sig/knowledge)。