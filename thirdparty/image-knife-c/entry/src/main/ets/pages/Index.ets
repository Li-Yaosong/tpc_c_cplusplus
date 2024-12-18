/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import { router } from '@kit.ArkUI'


@Entry
@Component
struct Index {

  getResourceString(res:Resource){
    return getContext().resourceManager.getStringSync(res.id)
  }


  aboutToAppear(): void {

  }

  build() {
    Scroll(){
      Column() {
        Button("单个图片").onClick(()=>{
          router.pushUrl({
            url: 'pages/SingleImage',
          });
        })
        Button("本地图片").margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/LocalImage',
          });
        })
        Button("Image-ObjectFit").margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/TestObjectFit',
          });
        })
        Button("边框与圆角").margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/TestBorder',
          });
        })
        Button("LazyForEach").margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/LazyForEach',
          });
        })
        Button("Reuse + LazyForEach").margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/ReuseLazyForEach',
          });
        })
        Button('缩放图片').margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/TransformPage',
          });
        })
        Button('prefetch+cachecount动态预加载').margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/PrefetchAndPreload',
          });
        })
        Button('ImageFit.Auto长图显示').margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/LongImagePage',
          });
        })
        Button('ImageFit.Auto自适应图片高度').margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/AutoImageFit',
          });
        })
        Button('测试Listner回调数量').margin({top:10}).onClick(()=>{
          router.pushUrl({
            url: 'pages/MultipleImageCallBack',
          });
        })
      }
    } .width('100%')
    .height('100%')
  }
}