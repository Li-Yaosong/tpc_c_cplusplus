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
import { BasicPrefetcher } from '@kit.ArkUI';
import { dataArray } from './model/ImageUrlData';
import { ImageKnifeComponent } from '@ohos/imageknifec';
import DataSourcePrefetchingImageKnife from './model/DataSourcePrefetching';

@Entry
@Component
export struct PrefetchAndPreload {
  // 创建DataSourcePrefetchingImageKnife对象，具备任务预取、取消能力的数据源
  private readonly dataSource = new DataSourcePrefetchingImageKnife(dataArray);
  // 创建BasicPrefetcher对象，默认的动态预取算法实现
  private readonly prefetcher = new BasicPrefetcher(this.dataSource);

  build() {
    Column() {
      List({ space: 16 }) {
        LazyForEach(this.dataSource, (item: string, index: number) => {
          ListItem() {
            Column({ space: 12 }) {
              ImageKnifeComponent({
                imageKnifeOption: {
                  loadSrc: item,
                  placeholderSrc: $r('app.media.loading')
                }
              }).width(100).height(100)
              Text(`${index}`)
            }.border({ width: 5, color: '#000000' })
          }
          .reuseId('imageKnife')
        })
      }
      .cachedCount(5)
      .onScrollIndex((start: number, end: number) => {
        // 列表滚动触发visibleAreaChanged，实时更新预取范围，触发调用prefetch、cancel接口
        this.prefetcher.visibleAreaChanged(start, end)
      })
      .width('100%')
      .height('100%')
      .margin({ left: 10, right: 10 })
      .layoutWeight(1)
    }
  }
}