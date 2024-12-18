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
import { ImageKnifeComponent } from '@ohos/imageknifec';

@Entry
@Component
struct TestObjectFit {
  build() {
    Scroll() {
      Column() {
        Text("左侧图：1050px*1894px  右侧图：268px * 140px")
          .font({size:16,weight:FontWeight.Bolder})
          .fontColor("#ff0000")
        Text("Contain-图片宽高等比例缩放至全部居中展示")
          .margin({top:10,bottom:10})
        Flex() {
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.pngSample'),
                objectFit:ImageFit.Contain,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.test'),
                objectFit:ImageFit.Contain,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")

        }
        Text("Cover-保持图片宽高比不变，短的部分缩放至组件大小，超出部分全部裁剪")
          .margin({top:10,bottom:10})
        Flex() {
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.pngSample'),
                objectFit:ImageFit.Cover,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.test'),
                objectFit:ImageFit.Cover,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
        }
        Text("Fill-不保持宽高比，进行缩放，图片充满显示边界")
          .margin({top:10,bottom:10})
        Flex() {
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.pngSample'),
                objectFit:ImageFit.Fill,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.test'),
                objectFit:ImageFit.Fill,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
        }
        Text("None-图像显示居中展示，不缩放")
          .margin({top:10,bottom:10})
        Flex() {
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.pngSample'),
                objectFit:ImageFit.None,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.test'),
                objectFit:ImageFit.None,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
        }
        Text("ScaleDown-图像大于组件则执行Contain，小于组件则None")
          .margin({top:10,bottom:10})
        Flex() {
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.pngSample'),
                objectFit:ImageFit.ScaleDown,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
          ImageKnifeComponent(
            {
              imageKnifeOption: {
                loadSrc: $r('app.media.test'),
                objectFit:ImageFit.ScaleDown,
                border:{color:"#000000",width:1}
              }
            }
          ).height(px2vp(200)).width("50%")
        }
        Text("图片组件宽高:50% * 200px")
          .font({size:20,weight:FontWeight.Bolder})
          .fontColor("#ff0000")
          .margin({top:10,bottom:10})
      }
    }.width("100%")
    .height("100%")
  }
}