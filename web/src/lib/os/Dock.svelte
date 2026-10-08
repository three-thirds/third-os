<script lang="ts">
    import { apps } from './apps';
    import { wm } from './windows.svelte';
</script>

<nav class="relative z-30 mx-auto mb-4 flex gap-3 rounded-2xl border border-bronze-600 p-3 backdrop-blur">
    {#each Object.values(apps) as app}
    {/each}
</nav>