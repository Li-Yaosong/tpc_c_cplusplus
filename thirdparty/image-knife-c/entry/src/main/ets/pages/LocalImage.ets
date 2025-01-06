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
import fs from '@ohos.file.fs';

@Entry
@Component
struct LocalImage {
  scroller: Scroller = new Scroller;
  localFile: string = getContext(this).filesDir + '/icon.png'

  aboutToAppear(): void {
    // 拷贝本地文件
    let icon: Uint8Array = getContext(this).resourceManager.getMediaContentSync($r('app.media.startIcon'));
    let file = fs.openSync(this.localFile, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
    fs.writeSync(file.fd, icon.buffer);
    fs.fsyncSync(file.fd);
    fs.closeSync(file);
  }

  build() {
    Scroll(this.scroller) {
      Column() {
        Text($r('app.string.local_r_file'))
          .fontSize(30)
          .fontWeight(FontWeight.Bold)
        ImageKnifeComponent({
          imageKnifeOption: {
            loadSrc: $r('app.media.startIcon'),
            objectFit: ImageFit.Contain
          }
        }).width(100).height(100)
        Text($r('app.string.local_rawfile'))
          .fontSize(30)
          .fontWeight(FontWeight.Bold)
        ImageKnifeComponent({
          imageKnifeOption: {
            loadSrc: $rawfile("image/startIcon.png"),
            objectFit: ImageFit.Contain
          }
        }).width(100).height(100)
        //Image($rawfile("image/startIcon.png")).width(100).height(100)
        Text($r('app.string.Under_context_file'))
          .fontSize(30)
          .fontWeight(FontWeight.Bold)
        ImageKnifeComponent({
          imageKnifeOption: {
            loadSrc: this.localFile,
            objectFit: ImageFit.Contain
          }
        }).width(100).height(100)
        Text($r('app.string.local_other_module'))
          .fontSize(30)
          .fontWeight(FontWeight.Bold)
        ImageKnifeComponent({
          imageKnifeOption: {
            loadSrc: $r('[sharedlibrary].media.startIcon'),
            objectFit: ImageFit.Contain
          }
        }).width(100).height(100)
      }
      .width('100%')
    }
    .height('100%')
  }
}

