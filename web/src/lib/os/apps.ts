import type { Component } from 'svelte';
import Sysinf from '../apps/Sysinf.svelte';
import appleIcon from '../assets/apple.svg?raw';
import Music from '../apps/Music.svelte';
import Docs from '../apps/docs/Docs.svelte';

export type AppDef = {
    id: string;
    title: string;
    icon: string;
    component: Component;
    w: number;
    h: number;
    single?: boolean;
}

const musicIcon = 'data:image/svg+xml,%3Csvg%20xmlns=%22http://www.w3.org/2000/svg%22%20width=%2224%22%20height=%2224%22%20viewBox=%220%200%2024%2024%22%20fill=%22none%22%20stroke=%22currentColor%22%20stroke-width=%222%22%20stroke-linecap=%22round%22%20stroke-linejoin=%22round%22%20class=%22icon%20icon-tabler%20icons-tabler-outline%20icon-tabler-music%22%3E%3Cpath%20stroke=%22none%22%20d=%22M0%200h24v24H0z%22%20fill=%22none%22%20/%3E%3Cpath%20d=%22M3%2017a3%203%200%201%200%206%200a3%203%200%200%200%20-6%200%22%20/%3E%3Cpath%20d=%22M13%2017a3%203%200%201%200%206%200a3%203%200%200%200%20-6%200%22%20/%3E%3Cpath%20d=%22M9%2017v-13h10v13%22%20/%3E%3Cpath%20d=%22M9%208h10%22%20/%3E%3C/svg%3E'

export const apps: Record<string, AppDef> = {
    sysinf: { id: 'sysinf', title: 'System Info', icon: 'https://img.icons8.com/ios-glyphs/60/leaf.png', component: Sysinf, w: 400, h: 315, single: true },
    music: { id: 'music', title: 'Music', icon: musicIcon, component: Music, w: 400, h: 600, single: true },
    files: { id: 'files', title: 'Files', icon: appleIcon, component: Docs, w: 1000, h: 600, single: true },
}