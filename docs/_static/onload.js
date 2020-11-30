window.onload = () => {
	console.log('Loaded.');

	const lis = document.getElementsByTagName('li');
	for (const li of lis)
		if (typeof li.className === 'string'
		&& li.className.includes('collapsibleListClosed'))
			li.click();
};
