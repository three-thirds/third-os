<script lang="ts">
    import { onMount } from "svelte"
    import QRCode from 'qrcode'
    import tux from '#lib/assets/tux.txt?raw'

    const url = 'https://www.youtube.com/watch?v=Aq5WXmQQooo';

    let qr = $state('');
    let ready = $state(false);

    onMount(() => {
        QRCode.toDataURL(url, {
            width: 500,
            margin: 0,
            errorCorrectionLevel: 'L',
            color: { dark: '#0000aa', light: '#ffffff' }
        }).then((data) => (qr = data))

        const t = setTimeout(() => (ready = true), 1000);
        return () => clearTimeout(t)
    })
</script>

<div class="fixed inset-0 z-100 bg-[#0000aa] p-6 font-mono text-[#f7f7fc]">
      <pre class="absolute left-6 top-4 text-[10px] leading-[1.05]">{tux}</pre>
      <div>
        <img src={qr} alt="QR code" class="bg-white " style="width: min(62vh, 85vw); aspect-ratio: 1; image-rendering: pixelated;"/>
      </div>
</div>