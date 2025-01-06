# ImageKnifeC

## 简介

ImageKnifeC是一个OpenHarmony/HarmonyOS图片加载缓存库，特点如下：
- 提供js api和c api两种使用方式：既提供js组件方式给arkts原生应用使用，也提供Native组件的方式给RN等框架映射使用。
- 内置使用系统网络KIT CAPI下载和系统Image CAPI渲染，图片加载更快。
- 内置内存和文件两级缓存。
- 支持通过拦截器的方式，自定义网络下载，解码，缓存策略，缓存key(**目前仅提供capi自定义拓展**)。
- 支持预加载图片。

## 下载安装
尚未发布OpenHarmony中心仓，暂时只提供源码引入方式：把library目录拷贝到应用目录下使用。

如果需要用文件缓存，需要提前初始化文件缓存
ImageKnife.GetInstance().InitFileCache(context, 256, 256 * 1024 * 1024)

## 使用方式
- arkts使用示例
```
ImageKnifeComponent(
    {
    imageKnifeOption: new ImageKnifeOption({
        loadSrc: "https://www.openharmony.cn/_nuxt/img/logo.dcf95b3.png", 
        placeholderSrc: "占位图",
        errorholderSrc："错误图"，
        onLoadListener:{
                onLoadStart:()=>{
                  console.log("onLoad start")
                },
                onLoadSuccess:(data,imageKnifeData)=>{
                  console.log("onLoad success: " + data)
                }
       },
       objectFit: ImageFit.Contain,
       border: { radius: { topLeft: 50, bottomRight: 50 } }// 圆角设置
    }),
    syncLoad:false // 是否同步加载
    }
).width('100%').height(200)
```
**注意：因为无法在c++子线程里执行ArkTs自定义下载，缓存，图形变化等方法，所以目前只能在c++侧进行自定义实现**

- c++使用示例

引入`imageKnife.h`头文件，在工程的napi init函数中对ImageKnife进行初始化，创建默认的imageLoader并设置为全局默认的loader。
loader包含了通过多种拦截器实现，提供图片加载，解码，缓存等能力。
```cpp
// 1.创建默认ImageLoader，或通过拦截器自定义Loader
auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
//2. 将创建的ImageLoader设置为全局的loader
ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);

//3.创建ImageKnifeOption和imageKnifeNode
auto imageKnifeOption = std::make_shared<ImageKnifeOption>();
auto imageKnifeNode = std::make_shared<ImageKnifeNode>(std::string(componentId), imageKnifeOption);

//4.挂载imageNode到页面上，并管理声明周期，如OH_ArkUI_NodeContent_AddNode

//5.执行请求，加载图片
ImageKnife::GetInstance().Execute(imageKnifeNode->GetImageKnifeRequest());

//6.销毁图片
// 将imageNode从页面中移除，释放node,option对象，如OH_ArkUI_NodeContent_RemoveNode
```
如果需要创建空的ImageKnifeLoader, 则使用`ImageKnifeC::ImageKnifeLoader::createEmptyImageLoader()`。
空的loader不包含默认的拦截器实现，可以后续添加指定的默认拦截器或自定义拦截器来实现下载解码等功能。

### 通过c++拦截器自定义实现
#### 拦截器用法
* 支持下载，内存缓存，文件缓存，解码4种拦截器扩展默认实现
* 每个拦截器需要继承对应的基类实现对应的虚函数，Resolve负责拦截器的功能实现，Cancel负责取消Resolve中的耗时动作。
* Resolve返回true则代表执行成功，责任链终止，进入下一个流程。返回false则代表执行失败，交由责任链上后一个拦截器。
* 拦截器需添加至ImageLoader中，当前只支持设置全局的一个ImageLoader，后续支持针对某个Option单独分别设置ImageLoader。

#### 拦截器传递数据ImageKnifeTask
* 拦截器通过`ImageKnifeTask`来传递参数和执行结构，其中`product`成员为拦截器生成的产品，执行结果需写回至`product`的成员上来传递至下一流程。
* `imageSrc`成员为当前加载任务对应的图片数据类型，可能是网络图片url 或本地Resource资源图（js层传入的pixelmap将直接进行图形变换后显示）
* 默认的下载拦截器下载的图片数据在`product.response`中，而内存磁盘缓存以及Resource加载的图片数据在`product.imageBuffer`中，并由`product.imageLength`标明图片长度。两者不混用
* `product.pixelmapCached`用于标识当前的`product.pixelmap`是否被缓存，如果当前的拦截器需要产出新的pixelmap,在没有被缓存的情况下，需要先使用`OH_PixelmapNative_Release(task->product.pixelmap);`来释放内存。
* 数据将在任务结束后自动释放内存。
```cpp
class ImageKnifeTask {
public:
     struct Product{
        // 网络下载session，用于取消下载
        Rcp_Session* session = nullptr;
        // 网络下载request，用于取消下载
        Rcp_Request* request = nullptr;
        // 网络下载, http响应数据
        Rcp_Response* response = nullptr;
        // 原始图片buffer, 
        uint8_t* imageBuffer = nullptr;
        // 原始图片长度
        uint32_t imageLength = 0;
        // pixelmap 图片
        OH_PixelmapNative* pixelmap = nullptr;
        // 标记是否任务结束后立即释放pixelmap 内存
        bool pixelmapCached = false;
        // 标记当前是需要写缓存还是读缓存
        bool writeCache = false;
    }
    // 拦截器执行结果
    Product product;
    // ImageKnife请求
    std::shared_ptr<ImageKnifeRequest> request;
    // 请求图片来源类型标识： 主图，错误图，占位图
    const ImageKnifeRequestSource type;
    // 请求图片数据类型标识： String | Pixelmap | Resource 
    const ImageData* imageSrc;
    // 错误信息
    std::string lastError;
    // 取消信息
    std::string cancelInfo;
};
```
#### 拦截器自定义数据
* `ImageKnifeOption`中预留了`void* customData`成员，用户可以将自定义数据类型赋给改成员，并在拦截器中通过`ImageKnifeTask`的`request->GetImageKnifeOption()->customData`取得。
* 通过设置`ImageKnifeOption`中的`std::function<void(void* customData)> onFinalize`成员来完成了对`customData`的析构以及其他操作。
  该函数指针将保证在整个图片请求（包含主图，占位图，错误图）完成后触发，即在请求失败，成功，取消结束后回调。



#### 拦截器示例
- 【c++】自定义图片下载
```cpp
class MyDownloadInterceptor: public ImageKnifeC::DownloadInterceptor{
public:
    bool Resolve(ImageKnifeTask* task) override
    {
        // 自实现网络请求，图片加载类型通过task->type 获取，数据通过task->product传递
        // 图片source信息由task->imageSrc描述
        std::string url;
        if (task->imageSrc->GetString(url)) {
            // 数据写入至product对应成员
            return loadImageFromUrl(url, task); // 如执行成功，下载责任链结束
        }
        
        // 执行失败或不支持，将执行责任链的下一个拦截器
        return false;
    }
    
};


EXTERN_C_START
static napi_value init(napi_env env, napi_value exports)
{
    // ImageKnifeC 初始化
    ImageKnifeC::ImageKnife::GetInstance().init(env, exports);
    // 创建默认ImageLoader
    auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
    
    // 添加自定义网络下载拦截器,默认添加至责任链开头
    imageLoader->AddDownloadInterceptor(std::make_shared<MyDownloadInterceptor>());
    // 将创建的ImageLoader设置为全局的loader
    ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);
    
//     napi_property_descriptor desc[] = {};
//     napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END
```

- 【c++】自定义解码图片
```cpp
class MyDecodeInterceptor: public ImageKnifeC::DecodeInterceptor{
public:
    bool Resolve(ImageKnifeTask* task) override
    {
        //自定义解码实现
        //产出可以写入 task->product.pixelmap
        //加载流程的产出可能在 task->product.response 或 task->product.imageBuffer中 
        return true;// 执行成功，责任链结束
        return false; // 执行失败或不支持，将执行责任链的下一个拦截器
    }
};

EXTERN_C_START
static napi_value init(napi_env env, napi_value exports)
{
    // ImageKnifeC 初始化
    ImageKnifeC::ImageKnife::GetInstance().init(env, exports);
    // 创建默认ImageLoader
    auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
    
    // 添加自定义解码图片拦截器,添加至责任链末尾
    imageLoader->AddDownloadInterceptor(std::make_shared<MyDecodeInterceptor>(), 
                                    ImageKnifeC::ImageKnifeLoader::Postion::END);
    // 将创建的ImageLoader设置为全局的loader
    ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);
    
//     napi_property_descriptor desc[] = {};
//     napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END
```
- 【c++】自定义内存缓存
```cpp
class MyMemoryInterceptor: public ImageKnifeC::MemoryCacheInterceptor{
public:
    
    bool Resolve(ImageKnifeTask* task) override
    {
        // 自定义缓存实现
        // task->product->writeCache 为true表示当前流程是需要写缓存，否则需要读缓存。
        // 内存缓存读取或写入使用 task->product->pixelmap
        return true;// 执行成功，责任链结束
        return false; // 执行失败或不支持，将执行责任链的下一个拦截器
    }
};

EXTERN_C_START
static napi_value init(napi_env env, napi_value exports)
{
    // ImageKnifeC 初始化
    ImageKnifeC::ImageKnife::GetInstance().init(env, exports);
    // 创建默认ImageLoader
    auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
    
    // 添加自定义内存缓存拦截器,添加至责任链末尾
    imageLoader->AddDownloadInterceptor(std::make_shared<MyMemoryInterceptor>(), 
                                    ImageKnifeC::ImageKnifeLoader::Postion::END);
    // 将创建的ImageLoader设置为全局的loader
    ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);
    
//     napi_property_descriptor desc[] = {};
//     napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END
```

- 【c++】自定义文件缓存
```cpp
class MyFileInterceptor: public ImageKnifeC::FileCacheInterceptor{
public:
    
    bool Resolve(ImageKnifeTask* task) override
    {
        // 自定义文件缓存实现
        // task->product->writeCache 为true表示当前流程是需要写缓存，否则需要读缓存。
        // 文件缓存写入来源使用task->response, 使用task->product->imageBuffer读取 
        return true;// 执行成功，责任链结束
        return false; // 执行失败或不支持，将执行责任链的下一个拦截器
    }
};

EXTERN_C_START
static napi_value init(napi_env env, napi_value exports)
{
    // ImageKnifeC 初始化
    ImageKnifeC::ImageKnife::GetInstance().init(env, exports);
    // 创建默认ImageLoader
    auto imageLoader = ImageKnifeC::ImageKnifeLoader::CreateDefaultImageLoader();
    
    // 添加自定义内存缓存拦截器,添加至责任链末尾
    imageLoader->AddDownloadInterceptor(std::make_shared<MyFileInterceptor>(), 
                                    ImageKnifeC::ImageKnifeLoader::Postion::END);
    // 将创建的ImageLoader设置为全局的loader
    ImageKnifeC::ImageKnife::GetInstance().SetDefaultImageKnifeLoader(imageLoader);
    
//     napi_property_descriptor desc[] = {};
//     napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END
```
- 【c++】自定义图形变化  
```
// 待定

```
#### border 设置边框圆角
暂时实现了border width与radius的支持，设置的数据类型为number,**其他类型尚未实现暂不支持**

**border color与style 尚未实现暂不支持**
```
imageKnifeOption: new ImageKnifeOption({
    loadSrc: $r('app.media.pngSample'),
    objectFit:ImageFit.Fill,
    border:{radius:50,width:1}  //圆角边框设置
})
```


## 与ImageKnife3.x特性对比
#### ImageKnifeOption
| 属性            |           ImageKnife3.x(js api)                                   | ImageKnifeC(js api)                      |ImageKnifeC(c api)                      |
| --------------------- | ----------------------------------------------------- | ------------------------------ |------------------------------ |
|  主图              | loadSrc:string, PixelMap, Resource                           | 一致                      |  |
|  占位图              | placeholderSrc?: string,PixelMap,Resource                         | 一致                      |  |
|  错误图              | errorholderSrc?: string,PixelMap,Resource                         | 一致                      |  |
|  http请求头              | headerOption?: Array<HeaderOptions>                        | 一致(待实现)                      |  |
|  自定义关键字              | signature?: string                     | 一致                      |  |
|  主图缩放效果              | objectFit?: ImageFit                   | 一致                      |  |
|  占位图缩放效果              | placeholderObjectFit?: ImageFit                   | 一致                      |  |
|  错误图缩放效果              | errorholderObjectFit?: ImageFit                   | 一致                      |  |
|  自定义下载方法              | customGetImage:function             | 不一致(待C侧实现)                      | 拦截器实现 |
|  圆角边框              | border?: BorderOptions                   | 一致                      |  |
|  写缓存策略              | writeCacheStrategy?: CacheStrategy                   | 一致(待实现)                      |  |
|  仅使用缓存加载              | onlyRetrieveFromCache?: boolean                   | 一致(待实现)                      |  |
|  请求优先级              | priority?: taskpool.Priority                   | 一致(待实现)                      |  |
|  页面上下文              | context?: common.UIAbilityContext                  | 一致                      |  |
|  下载进度监听              | progressListener?: (progress: number) => void                  | 一致(待实现)                      |  |
|  图形变化              | transformation?: PixelMapTransformation                 |不一致(待C侧实现)                      |  |
|  请求回调              | onLoadListener?: OnLoadCallBack,undefined)                | 一致                    |  |
|  渲染完成回调              | onComplete?:(event:EventImage,undefined) => void                | 一致(待实现)                     |  |
|  颜色修改              | drawingColorFilter?: ColorFilter,drawing.ColorFilter               | 一致(待实现)                     |  |
|  降采样策略              | downsampleOf?: DownsampleStrategy               | 一致(待实现)                     |  |
|  http ca证书             | caPath?: string               | 一致(待实现)                     |  |


#### ImageKnife全局方法
| 方法            |           ImageKnife3.x(js api)                                   | ImageKnifeC(js api)                      |ImageKnifeC(c api)                      |
| --------------------- | ----------------------------------------------------- | ------------------------------ |------------------------------ |
|  全局主线程加载              | isRequestInSubThread(value: boolean)                         | 一致(待实现)                      |  |
|  初始化文件缓存             | InitFileCache(context: Context, size: number = 256, memory: number = 256 * 1024 * 1024,path?: string)                         | 一致(待实现)                      |  |
| 判断文件缓存是否已完成初始化           | isFileCacheInit(): boolean                         | 一致(待实现)                      |  |
|  重新加载              | reload(request: ImageKnifeRequest)                         | 一致(待实现)                      |  |
|  全局添加单个请求头header              | addHeader(key: string, value: Object)                         | 一致(待确认)                      |  |
|  全局设置请求头header             | serHeaderOptions(options)                        | 一致(待实现)                      |  |
|  删除单个请求头header            | deleteHeader(key: string)                       | 一致(待实现)                      |  |
|  设置自定义的内存缓存            | initMemoryCache(newMemoryCache: IMemoryCache)                        | 不一致(拦截器实现)                      |  |
|  清除所有内存缓存             | removeAllMemoryCache()                        | 一致(待实现)                      |  |
|  清除指定内存缓存             | removeMemoryCache(url: string \|ImageKnifeOption)                         | 一致(待实现)                      |  |
|  预加载             | preload(loadSrc:string \| ImageKnifeOption)                        | 一致(待实现)                      |  |
|  取消图片请求             | Cancel(request:ImageKnifeRequest)                        | 一致(待实现)                      |  |
|  预加载图片到文件缓存,返回文件路径              | preLoadCache(loadSrc: string \| ImageKnifeOption)                        | 一致(待确认)                      |  |
|  从内存或文件缓存中获取图片数据             |   getCacheImage(loadSrc: string,cacheType: CacheStrategy = CacheStrategy.Default, signature?: string)  | 一致(待确认)                      |  |
|  预加载缓存（用于外部已获取pixelmap，需要加入imageknife缓存的场景）             | PutCacheImage(url: string, pixelMap: PixelMap, cacheType: CacheStrategy = CacheStrategy.Default, signature?: string)                       | 一致(待确认)                      |  |
|  清除所有文件缓存            | removeAllFileCache()                       | 一致(待实现)                      |  |
|  清除指定文件缓存            | removeFileCache(url: string \| ImageKnifeOption)                      | 一致(待实现)                      |  |
|  设置请求并发数量            | setMaxRequests(concurrency: number)                      | 一致(待实现)                      |  |
|  根据key从文件缓存获取buffer            | getFileCacheByFile(context: Context, key: string)                     | 一致(待实现)                      |  |
|  根据key从内存缓存获取pixelmap            | LoadFromMemoryCache(key: string)                     | 一致(待实现)                      |  |
|  设置请求并发数量            | setMaxRequests(concurrency: number)                      | 一致(待实现)                      |  |
|  获取缓存限制的大小            | getCacheLimitSize(cacheType?: CacheStrategy)                     | 一致(待实现)                      |  |
|  获取缓存限制的数量            | getCurrentCacheNum(cacheType: CacheStrategy)                      | 一致(待实现)                      |  |
|  获取当前缓存大小            | getCurrentCacheSize(cacheType: CacheStrategy)                      | 一致(待实现)                      |  |
|  自定义缓存key生成方法            | SetEngineKeyImpl(impl: IEngineKey)                      | 不一致(待C侧实现)                       |  |
|  全局设置自定义下载            | setCustomGetImage(...)                     | 不一致(待C侧实现)                     |  |
|  全局获取自定义下载            | getCustomGetImage()                   | 不一致(待C侧实现)                      |  |

#### 其它属性
| 属性            |           ImageKnife3.x(js api)                                   | ImageKnifeC(js api)                      |ImageKnifeC(c api)                      |
| --------------------- | ----------------------------------------------------- | ------------------------------ |------------------------------ |
|  同步加载              | syncLoad: boolean                          | 一致(待实现)                      |  |
|  动图控制及回调              | animatorOption: AnimatorOption                          | 一致(待实现)                      |  |
|  放大缩小等组件缩放              | Transform(matrix:object)                         | 一致(待实现)                      |  |
|  组件自适应图片宽高              | ImageFit.Auto                         | 一致(待实现)                      |  |






## Benchmark
待测试比较imageknife3.x,imageknifeC,系统image,安卓fresco/glide 加载图片速度和白块率。


## 开源协议

本项目基于 [Apache License 2.0](https://gitee.com/openharmony-tpc/ImageKnife/blob/master/LICENSE) ，请自由的享受和参与开源。

