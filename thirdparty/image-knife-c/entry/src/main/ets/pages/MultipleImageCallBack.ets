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
import { ImageKnifeComponent,ImageKnifeOption } from '@ohos/imageknifec';
import { LogUtil } from '../util/LogUtil';
import { CommonDataSource } from './model/CommonDataSource'

@Entry
@Component
struct MultipleImageCallBack {
  @State hotCommendList: CommonDataSource<string> = new CommonDataSource<string>([])
  @State componentIndex: number = 0
  @State startIndex: number = 0
  @State successIndex: number = 0
  @State failIndex: number = 0
  @State cancelJobIndex: number = 0
  @State cancelLoadIndex: number = 0
  @State memoryIndex: number = 0
  @State fileCacheIndex: number = 0
  @State netIndex: number = 0
  @State checkText: string = ''
  private data: Array<string> = []

  aboutToAppear(): void {
    for (let index = 0; index < 100; index++) {
      this.data.push(`https://img-blog.csdn.net/20140514114029140?${index}`);
    }
    this.hotCommendList.addData(this.hotCommendList.totalCount(), this.data);
  }

  build() {
    Column() {
      Row() {
        Column() {
          Text('图片总数：' + this.componentIndex)
          Text('开始回调：' + this.startIndex)
          Text('成功回调：' + this.successIndex)
          Text('失败回调：' + this.failIndex)
          Text('队列取消回调：' + this.cancelJobIndex)
          Text('加载取消回调：' + this.cancelLoadIndex)
          Text('内存数量：' + this.memoryIndex)
          Text('文件数量：' + this.fileCacheIndex)
          Text('网络数量：' + this.netIndex)
        }.width('50%')
        Column() {
          Button('check')
            .onClick(()=>{
              this.checkText = ''
              if (this.componentIndex !== this.startIndex + this.cancelJobIndex) {
                this.checkText = this.checkText + '图片总数!=开始+队列取消\r\n';
              }
              if(this.startIndex !== this.successIndex + this.failIndex + this.cancelLoadIndex) {
                this.checkText = this.checkText + '开始回调!=成功+失败+加载取消\r\n';
              }
              if(this.successIndex !== this.memoryIndex + this.fileCacheIndex + this.netIndex) {
                this.checkText = this.checkText + '成功回调!=内存+文件+网络';
              }
              if(this.checkText == '') {
                this.checkText = 'check正确';
              }
            })
          Text(this.checkText)
        }.width('50%')
      }.width('100%')
      Column() {
        WaterFlow() {
          LazyForEach(this.hotCommendList, (item: string,index: number) => {
            FlowItem() {
              Column() {
                Text(index + '')
                ImageComponent({
                  imageKnifeOption: {
                    loadSrc: item,
                    placeholderSrc: $r('app.media.loading'),
                    errorholderSrc: $r('app.media.failed'),
                    onLoadListener: {
                      onLoadStart:()=>{
                        this.startIndex++
                        LogUtil.info('image load multiple loadStart:' + this.startIndex)
                      },
                      onLoadSuccess:(pixelmap,imageData,request)=>{
                        this.successIndex++;
                        let memory = request?.imageKnifeData?.timeInfo?.memoryCheckEndTime ? 1 : 0;
                        let fileCache = request?.imageKnifeData?.timeInfo?.diskCheckEndTime ? 1 : 0;
                        let net = request?.imageKnifeData?.timeInfo?.netRequestEndTime ? 1 : 0;
                        memory = memory - fileCache;
                        fileCache = fileCache - net;
                        this.memoryIndex = this.memoryIndex + memory;
                        this.fileCacheIndex = this.fileCacheIndex + fileCache;
                        this.netIndex = this.netIndex + net;
                        LogUtil.info('image load multiple loadSuccess:' + this.successIndex);
                      },
                      onLoadFailed:()=>{
                        this.failIndex++
                        LogUtil.info('image load multiple loadFail:' + this.failIndex);
                      },
                      onLoadCancel:(message,request)=>{
                        let flag = request?.imageKnifeData?.type ? true : false;
                        if (flag) {
                          this.cancelLoadIndex++;
                        } else {
                          this.cancelJobIndex++;
                        }
                        LogUtil.info('image load multiple cancelJobIndex:' + this.cancelJobIndex,'cancelLoadIndex' + this.cancelLoadIndex);
                      }
                    }
                  },index:this.componentIndex
                }).width('50%').height(160)
              }
            }.height(200)
            .backgroundColor('#95efd2')
          }, (item: string) => item)
        }
        .cachedCount(0)
        .columnsTemplate('1fr 1fr')
        .columnsGap(10)
        .rowsGap(5)
        .backgroundColor(0xFAEEE0)
        .width('100%')
      }
      .height('80%')
    }.width('100%')
    .height('100%')
  }
}
@Component
struct ImageComponent {
  @State imageKnifeOption: ImageKnifeOption = new ImageKnifeOption()
  @Link index: number
  aboutToAppear(): void {
    this.index++
  }
  build() {
    ImageKnifeComponent({
      imageKnifeOption: this.imageKnifeOption
    })
  }
}