import { apps } from './apps'

export type WindowState = {
    id: string;
    appId: string;
    x: number;
    y: number;
    w: number;
    h: number;
    z: number;
    minimized: boolean;
    maximized: boolean;  
}

class WindowManager {
    wins = $state<WindowState[]>([]);
    #z = 1
    #n = 0

    isTop(win: WindowState) {
        const visible = this.wins.filter((x) => !x.minimized)
        return !win.minimized && win.z === Math.max(...visible.map((x) => x.z))
    }

    focus(id: string) {
        const w = this.wins.find((x) => x.id === id)
        if (w) w.z = ++this.#z
    }

    launch(appId: string) {
        const app = apps[appId]
        const existing = this.wins.find((w) => w.appId === appId)

        if (app.single && existing) {
            if (existing.minimized) {
                existing.minimized = false
                this.focus(existing.id)
            } else if (this.isTop(existing)) {
                existing.minimized = true
            } else {
                this.focus(existing.id)
            }
            return;
        }

        const off = (this.#n++ % 8) * 28
        this.wins.push({
            id: crypto.randomUUID(),
            appId,
            x: 120 + off,
            y: 80 + off,
            w: app.w,
            h: app.h,
            z: this.#z++,
            minimized: false,
            maximized: false
        })
    }

    close(id: string) {
        this.wins = this.wins.filter((x) => x.id !== id)
    }
}

export const wm = new WindowManager()