import type { Component } from 'svelte';
import Sysinf from '../apps/Sysinf.svelte';
import appleIcon from '../assets/apple.svg?raw';

export type AppDef = {
    id: string;
    title: string;
    icon: string;
    component: Component;
    w: number;
    h: number;
    single?: boolean;
}

export const apps: Record<string, AppDef> = {
    sysinf: { id: 'sysinf', title: 'System Info', icon: 'https://img.icons8.com/ios-glyphs/60/leaf.png', component: Sysinf, w: 400, h: 315, single: true },
}