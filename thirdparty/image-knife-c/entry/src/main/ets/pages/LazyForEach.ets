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
import { CommonDataSource } from './model/CommonDataSource';
import { dataArray } from './model/ImageUrlData';

@Entry
@ComponentV2
export struct LazyForEachCount {
  @Local hotCommendList:CommonDataSource<string> = new CommonDataSource<string>([])
  aboutToAppear(): void {
    this.hotCommendList.addData(this.hotCommendList.totalCount(),dataArray)
  }
  build() {
    Column() {
      WaterFlow() {
        LazyForEach(this.hotCommendList,(item: string,index:number)=>{
          FlowItem() {
            Column(){
              ImageKnifeComponent({
                imageKnifeOption: {
                  loadSrc: item,
                  placeholderSrc: $r("app.media.loading"),
                  errorholderSrc: $r("app.media.failed"),
                }
              }).width("50%").height(160)
              Text(index + "")
            }
          }.height(200)
          .backgroundColor("#95efd2")
        },(item: string) => item)
      }.columnsTemplate("1fr 1fr")
      .columnsGap(10)
      .rowsGap(5)
      .backgroundColor(0xFAEEE0)
      .width("100%").height("100%")
    }
  }
}