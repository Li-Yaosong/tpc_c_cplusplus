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
struct AutoImageFit {
  @State width1: Length = '100%'

  build() {
    Scroll() {
      Column() {
        Button($r('app.string.adjust_size')).onClick(() => {
          if (this.width1.toString() == '100%') {
            this.width1 = '60%';
          } else {
            this.width1 = '100%';
          }
        }).width('100%')
        Text('Image')
        Image('https://contentcenter-drcn.dbankcdn.cn/pub_1/DevEcoSpace_1_900_9/56/v3/8MdhfSsCSMKj4sA6okUWrg/5uBx56tLTUO3RYQl-E5JiQ.jpg')
          .width('100%')
          .objectFit(ImageFit.Auto)
        Text('ImageKnife')
        ImageKnifeComponent({
          imageKnifeOption: {
            loadSrc: 'https://contentcenter-drcn.dbankcdn.cn/pub_1/DevEcoSpace_1_900_9/56/v3/8MdhfSsCSMKj4sA6okUWrg/5uBx56tLTUO3RYQl-E5JiQ.jpg',
            objectFit: ImageFit.Contain,
          }
        }).width('100%')

      }.width(this.width1).border({ width: 1 })
    }
  }
}


