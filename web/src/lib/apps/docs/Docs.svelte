<script lang="ts">
    import { fly, scale } from "svelte/transition"

    type FileItem = { name: string; content: string}
    type Folder = { id: string; name: string; files?: FileItem[]}

    const folders: Folder[] = [
        {
            id: 'music',
            name: 'Music',
            files: [
                { name: 'song1.mp3', content: 'This is song 1' },
                { name: 'song2.mp3', content: 'This is song 2' },
            ]
        }
    ]

    let folder = $state<Folder | null>(null)
    let file = $state<FileItem | null>(null)

    function openFolder(f: Folder) {
        if(!f.files ) return;
        folder = f;
        file = null;
    }

    function back() {
        if (file) file = null;
        else folder = null;
    }
</script>

<div class="size-full cursor-default overflow-auto p-6"
    style="background-image: radial-gradient(#d589361a 1px, transparent 1.5px); background-size: 16px 16px">

    {#if !folder}
        hi
        {#each folders as f, i}
        <button onclick={() => openFolder(f)}>
            {f.name}
        </button>
        {/each}
    {:else}
        ur in a folder
    {/if}

</div>