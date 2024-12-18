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
import { ImageKnifeComponent, ImageKnifeOption } from '@ohos/imageknifec'

@Entry
@Component
struct TestBorder {
  imageRadius: number = 50
  imageWidth: number = 1
  @State imageKnifeOption: ImageKnifeOption = new ImageKnifeOption()

  build() {
    Column({ space: 20 }) {
      Row() {
        Text('Radius:').fontSize(20)
        TextArea({
          text: this.imageRadius.toString()
        }).type(TextAreaType.NUMBER)
          .onChange((value: string) => {
            this.imageRadius = parseInt(value)
          })
          .width(150).fontSize(20)
      }

      Row() {
        Text('Width:').fontSize(20)
        TextArea({
          text: this.imageWidth.toString(),
        }).type(TextAreaType.NUMBER)
          .onChange((value: string) => {
            this.imageWidth = parseInt(value)
          })
          .width(150).fontSize(20)
      }

      Button("设置边框").onClick(() => {
        this.imageKnifeOption = {
          loadSrc: "https://www.openharmony.cn/_nuxt/img/logo.dcf95b3.png",
          placeholderSrc: $r('app.media.loading'),
          errorholderSrc: $r('app.media.failed'),
          onLoadListener: {
            onLoadStart: () => {
              console.log("onLoad start(net-resource)")
            },
            onLoadFailed: (err) => {
              console.error("onLoad onLoadFailed(net-resource) " + err)
            },
            onLoadSuccess: (data, imageKnifeData) => {
              console.log("onLoad onLoadSuccess(net-resource)")
            }
          },
          objectFit: ImageFit.Contain,
          border: { radius: this.imageRadius, width: this.imageWidth }
        }
      }).fontSize(20).padding(10)

      ImageKnifeComponent(
        {
          imageKnifeOption: this.imageKnifeOption
        }
      ).width(200).height(200)
    }
    .height('100%')
    .width('100%')
    .padding(20)
  }
}