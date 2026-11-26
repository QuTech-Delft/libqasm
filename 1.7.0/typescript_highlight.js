document$.subscribe(() => {
    hljs.highlightAll(),
        { language: 'typescript' }
})
