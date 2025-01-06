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
import matrix4 from '@ohos.matrix4'

@Entry
@Component
struct TransformPage {
  private custom_scale: number = 1
  @State matrix1: object = matrix4.identity().scale({ x: 1, y: 1 })
  @State ImageKnifeOption: ImageKnifeOption = {
    loadSrc: $r('app.media.test'),
    placeholderSrc: $r('app.media.loading'),
    errorholderSrc: $r('app.media.app_icon'),
    objectFit: ImageFit.Contain,
    border: { radius: 50 }
  }

  build() {
    Column() {
      ImageKnifeComponent({ imageKnifeOption: this.ImageKnifeOption }).height(200).width(200)
        .transform(this.matrix1)
      // Image($r('app.media.rabbit')).objectFit(ImageFit.Contain).height(200).width(200).transform(this.matrix1)
      Button('放大').onClick(() => {
        this.custom_scale = this.custom_scale * 2
        this.matrix1 = matrix4.identity().scale({ x: this.custom_scale, y: this.custom_scale })
      })

      Button('缩小').onClick(() => {
        this.custom_scale = this.custom_scale / 2
        this.matrix1 = matrix4.identity().scale({ x: this.custom_scale, y: this.custom_scale })
      })
    }
    .width('100%')

    .height('100%')
  }
}