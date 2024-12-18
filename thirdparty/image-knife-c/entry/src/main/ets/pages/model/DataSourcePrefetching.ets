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
import { ImageKnife, ImageKnifeRequest } from '@ohos/imageknifec'
import { IDataSourcePrefetching } from '@kit.ArkUI';
import { HashMap } from '@kit.ArkTS';
import { LogUtil } from '../../util/LogUtil';

export default class DataSourcePrefetchingImageKnife implements IDataSourcePrefetching {
  private dataArray: Array<string>;
  private readonly requestList: HashMap<number, ImageKnifeRequest> = new HashMap();
  private listeners: DataChangeListener[] = [];

  constructor(dataArray: Array<string>) {
    this.dataArray = dataArray;
  }

  public getData(index: number) {
    return this.dataArray[index];
  }

  public totalCount(): number {
    return this.dataArray.length;
  }

  public addData(index: number, data: string[]): void {
    this.dataArray = this.dataArray.concat(data);
    this.notifyDataAdd(index);
  }

  unregisterDataChangeListener(listener: DataChangeListener): void {
    const pos = this.listeners.indexOf(listener);
    if (pos >= 0) {
      this.listeners.splice(pos, 1);
    }
  }

  registerDataChangeListener(listener: DataChangeListener): void {
    if (this.listeners.indexOf(listener) < 0) {
      this.listeners.push(listener);
    }
  }

  notifyDataAdd(index: number): void {
    this.listeners.forEach((listener: DataChangeListener) => {
      listener.onDataAdd(index);
    })
  }

  async prefetch(index: number): Promise<void> {
    let item = this.dataArray[index];
    LogUtil.info("DataSourcePrefetchingImageKnife preload: index = %{public}d , loadsrc = %{public}s", index, item);
    // 图片预加载
    let request = ImageKnife.getInstance().preload({
      loadSrc: item,
      onLoadListener: {
        onLoadSuccess: () => {
          // 预加载成功，删除成功请求
          this.requestList.remove(index);
        },
        onLoadFailed: () => {
          // 移除失败请求
          this.requestList.remove(index);
        }
      }
    })
    this.requestList.set(index, request);
  }

  // 取消请求处理
  cancel(index: number) {
    if (this.requestList.hasKey(index)) {
      // 返回MAP对象指定元素
      const request = this.requestList.get(index);
      LogUtil.info("DataSourcePrefetchingImageKnife cancel: index = %{public}d , loadsrc = %{public}s", index,
        request?.imageKnifeOption?.loadSrc);
      // 取消请求
      ImageKnife.getInstance().cancel(request);
      // 移除被取消的请求对象
      this.requestList.remove(index);
    }
  }
}