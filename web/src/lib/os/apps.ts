import type { Component } from 'svelte';
import Sysinf from '../apps/Sysinf.svelte';

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
    sysinf: { id: 'sysinf', title: 'System Info', icon: 'sysinf', component: Sysinf, w: 400, h: 300, single: true },
}