<script lang="ts">
    import { onMount } from "svelte"
    import QRCode from 'qrcode'
    import tux from '#lib/assets/tux.txt?raw'
    import roasts from '#lib/assets/roasts.json'

    const roast = roasts[Math.floor(Math.random() * roasts.length)];

    const url = 'https://www.youtube.com/watch?v=Aq5WXmQQooo';

    let qr = $state('');
    let ready = $state(false);

    onMount(() => {
        QRCode.toDataURL(url, {
            width: 500,
            margin: 2,
            errorCorrectionLevel: 'L',
            color: { dark: '#0000aa', light: '#ffffff' }
        }).then((data) => (qr = data))

        const t = setTimeout(() => (ready = true), 1000);
        return () => clearTimeout(t)
    })
</script>

<div class="fixed inset-0 z-100 overflow-hidden bg-[#0000aa] p-6 font-mono text-[#f7f7fc]">
  <pre class="absolute left-6 top-4 text-[10px] leading-[1.05]">{tux}</pre>

  <div class="flex h-full flex-col items-center justify-center gap-8 py-6 text-center">
    {#if qr}
      <img
        src={qr}
        alt="QR code"
        style="width: min(55vh, 85vw); aspect-ratio: 1; image-rendering: pixelated;"
      />
    {/if}

    <div class="space-y-3 text-sm md:text-base">
      <p class="font-bold">KERNEL PANIC!</p>
      <p>{roast}</p>
    </div>
  </div>
</div>