<script lang="ts">
  import Folderthing from './folder.svelte';
  import Home from './home.svelte';
  import { folders, type Folder, type FileItem } from './data';

  let folder = $state<Folder | null>(null);
  let file = $state<FileItem | null>(null);

  function openFolder(f: Folder) {
    if (!f.files) return;
    folder = f;
    file = null;
  }

  function back() {
    if (file) file = null;
    else folder = null;
  }
</script>

<div
  class="size-full cursor-default overflow-auto p-6 text-bronze-900"
  style="background-image: radial-gradient(#d589361a 1px, transparent 1.5px); background-size: 16px 16px"
>
  {#if !folder}
    <Home {folders} onOpen={openFolder} />
  {:else}
    <Folderthing {folder} {file} onFile={(f) => (file = f)} onBack={back} />
  {/if}
</div>