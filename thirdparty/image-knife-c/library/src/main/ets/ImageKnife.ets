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
import { ImageKnifeRequest } from './model/ImageKnifeRequest';
import nativeNode from 'libimageknifec.so';
import { ImageKnifeOption } from './model/ImageKnifeOption';
import { common } from '@kit.AbilityKit';

export class ImageKnife {
  private static instance: ImageKnife;

  private constructor() {

  }

  public static getInstance(): ImageKnife {
    if (!ImageKnife.instance) {
      ImageKnife.instance = new ImageKnife();
    }
    return ImageKnife.instance;
  }

  /**
   * 初始化文件缓存个数，大小，以及路径
   * @param context 上下文
   * @param size 缓存数量
   * @param memory 内存大小
   * @param path 文件目录
   */
  async initFileCache(context: Context, size: number = 256, memory: number = 256 * 1024 * 1024,path?: string): Promise<string>  {
    return await nativeNode.initFileCache(context.filesDir, size, memory, path);
  }

  /**
   * 设置最大请求数量
   * @param concurrency 请求数量
   */
  public setMaxRequests(concurrency: number): void {
    nativeNode.setMaxRequests(concurrency);
  }

  /**
   * 预加载
   * @param loadSrc 图片地址url
   * @returns 图片请求request
   */
  preload(loadSrc: string | ImageKnifeOption): ImageKnifeRequest {
    let imageKnifeOption = new ImageKnifeOption()
    if (typeof loadSrc == 'string') {
      imageKnifeOption.loadSrc = loadSrc
    } else {
      imageKnifeOption = loadSrc;
    }
    // 创建 ImageKnifeRequest 实例，其他参数为空或使用默认值
    const request = new ImageKnifeRequest(
      imageKnifeOption,
      imageKnifeOption.context !== undefined ? imageKnifeOption.context : getContext() as common.UIAbilityContext,
      0,
      0,
      0
    );
    return nativeNode.preload(request)
  }
  /**
   * 取消图片请求
   * @param request 图片请求request
   */
  cancel(request: ImageKnifeRequest): void {
    nativeNode.cancel(request)
  }




  async execute(request: ImageKnifeRequest,isAnimator?: boolean): Promise<void> {

  }
}