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
import { ImageKnifeComponent, ImageKnifeOption } from '@ohos/imageknifec';

@Entry
@Component
struct SingleImage {
  build() {
    Row() {
      Column() {
        Text("网络资源").fontSize(30).fontWeight(FontWeight.Bold)
        ImageKnifeComponent(
          {
            imageKnifeOption: {
              loadSrc: "https://www.openharmony.cn/_nuxt/img/logo.dcf95b3.png",
              placeholderSrc: $r('app.media.loading'),
              errorholderSrc: $r('app.media.failed'),
              onLoadListener:{
                onLoadStart:()=>{
                  console.log("onLoad start(net-resource)")
                },
                onLoadFailed:(err)=>{
                  console.error("onLoad onLoadFailed(net-resource) " + err)
                },
                onLoadSuccess: (data, imageKnifeData) =>{
                  console.log("onLoad onLoadSuccess(net-resource)")
                }
              },
              objectFit:ImageFit.Contain,
            }
          }
        ).width(100).height(100)


        Text("占位图显示").fontSize(30).fontWeight(FontWeight.Bold).margin({top:10})
        ImageKnifeComponent(
          {
            imageKnifeOption: {
              loadSrc: "https://github.com/taglib/taglib/releases/download/v2.0.2/taglib-2.0.2.tar.gz",
              placeholderSrc: $r('app.media.loading'),
              objectFit:ImageFit.Contain,
              onLoadListener:{
                onLoadStart:()=>{
                  console.log("onLoad start(net-resource)")
                },
                onLoadFailed:(err)=>{
                  console.error("onLoad onLoadFailed(net-resource) " + err)
                },
                onLoadSuccess: (data, imageKnifeData) =>{
                  console.log("onLoad onLoadSuccess(net-resource)")
                }
              },
            }
          }
        ).width(100).height(100)
      }
      .width('100%')
    }
    .height('100%')
  }
}
