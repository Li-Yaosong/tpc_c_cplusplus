import { ImageKnifeRequest } from '../../../ets/model/ImageKnifeRequest';
import { ImageKnifeOption } from '../../../ets/model/ImageKnifeOption';

export const createNativeRoot: (content: Object , componentId: string, imageKinfeOption: ImageKnifeOption, syncLoad: boolean) => void;
export const updateNativeRoot: (componentId: string, imageKinfeOption: Object, componentVersion: number, syncLoad: boolean) => void;
export const clearNativeRoot: (componentId: string) => void;
export const destroyNativeRoot: (componentId: string) => void;
export const initFileCache: (filesDir: string, size: number, memory: number, path?: string) => Promise<string>;
export const setMaxRequests: (concurrency: number) => void;
export const preload: (imageKnifeRequest: ImageKnifeRequest) => ImageKnifeRequest;
export const cancel: (request: ImageKnifeRequest)=> void;
