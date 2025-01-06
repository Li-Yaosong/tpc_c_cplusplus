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


@Component
export struct UserAvatar {
  @Prop @Watch('userInfoUpdate') userInfo: string = ''
  // @Prop userInfo: string = ''
  imgSize: number = 100
  radius: number = 12
  borderSize: number = 0
  imgSizes: number = 1
  @State imageKnifeOption: ImageKnifeOption = new ImageKnifeOption()
  @StorageProp('WeLink_Mob_fontSize_multiple') @Watch('updateImgSize') WeLink_Mob_fontSize_multiple: number = 0
  scalable: boolean = true;
  @State calcImgSize: number = 100

  aboutToAppear(): void {
    this.userInfoUpdate()
    this.setImageSize()
  }

  setImageSize() {
    if (!this.scalable) {
      this.calcImgSize = this.imgSize
    } else if (this.WeLink_Mob_fontSize_multiple < 0.9) {
      this.calcImgSize = this.imgSize * 0.9
    } else if (this.WeLink_Mob_fontSize_multiple > 1.6) {
      this.calcImgSize = this.imgSize * 1.6
    } else {
      this.calcImgSize = this.imgSize * this.WeLink_Mob_fontSize_multiple
    }
  }

  updateImgSize() {
    this.setImageSize()
  }

  aboutToReuse(param: ESObject) {
    this.userInfoUpdate()
  }

  userInfoUpdate() {
    this.imageKnifeOption = {
      //TODO:写死loadSRC，场景：变更组件大小，所有图片不显示
      loadSrc: this.userInfo,
      placeholderSrc: $r('app.media.loading'),
      errorholderSrc: $r('app.media.failed'),
      objectFit:ImageFit.Contain
    }
  }

  build() {
    ImageKnifeComponent({ imageKnifeOption: this.imageKnifeOption })
      .borderRadius(this.radius)
      .clip(true)
      .width(this.calcImgSize)
      .height(this.calcImgSize)
      .backgroundColor(Color.Pink)
  }
}