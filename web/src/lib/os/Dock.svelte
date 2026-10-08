<script lang="ts">
    import { apps } from './apps';
    import { wm } from './windows.svelte';
</script>

<nav class="relative z-30 mx-auto mb-4 flex gap-3 rounded-2x p-3 backdrop-blur rounded-full">
    {#each Object.values(apps) as app}
        {@const running = wm.wins.some(win => win.appId === app.id)}
        <button
            class="relative grid size-12 place-items-center rounded-xl"
            title={app.title}
            aria-label={app.title}
            onclick={() => wm.launch(app.id)}
        >
            <span class="size-10 [&>svg]:size-full">{@html app.icon}</span>
        </button>
    {/each}
</nav>