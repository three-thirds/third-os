<script lang="ts">
    import { apps } from './apps';
    import { wm } from './windows.svelte';
</script>

{#each Object.values(apps) as app}
    <button title={app.title} onclick={() => wm.launch(app.id)}>{app.title}</button>
{/each}