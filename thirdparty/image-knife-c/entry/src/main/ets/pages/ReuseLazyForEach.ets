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
import { CommonDataSource } from './model/CommonDataSource'
import { UserAvatar } from './model/User'
import { dataArray } from './model/ImageUrlData';

@Entry
@Component
struct Index {
  @State hotCommendList:CommonDataSource<string> = new CommonDataSource<string>([])
  aboutToAppear(): void {
    this.hotCommendList.addData(this.hotCommendList.totalCount(), dataArray)
    AppStorage.set('WeLink_Mob_fontSize_multiple', 1)
  }

  build() {
    Column() {
      Button('bigger').onClick(()=>{
        AppStorage.set('WeLink_Mob_fontSize_multiple',1.6)
      })
      Button('small').onClick(()=>{
        AppStorage.set('WeLink_Mob_fontSize_multiple',0.8)
      })
      List(){
        LazyForEach(this.hotCommendList,(item:string)=>{
          ListItem(){
            ReuseImage({
              userInfo:item
            }).width('100%').height('100%').backgroundColor(Color.Yellow)
          }.width(200).height(200).margin({bottom:5})
        })
      }
      .cachedCount(3)
      .width('100%')
      .height('100%')
      .backgroundColor(0xFAEEE0)
    }.width('100%').height('100%')
  }
}


@Reusable
@Component
struct ReuseImage {
  @State userInfo:string = ''
  aboutToReuse(params: ESObject): void {
    this.userInfo = params.userInfo
  }

  build() {
    Column(){
      UserAvatar({
        userInfo:this.userInfo
      })
    }.width('100%').height('100%')
  }
}